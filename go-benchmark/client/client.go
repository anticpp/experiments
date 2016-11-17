package main

import (
	"flag"
	"fmt"
	"github.com/anticpp/experiments/go-benchmark/pack"
	"net"
	"sync"
	"sync/atomic"
	"time"
)

type statInfo struct {
	completeRequests int32
	failedRequests   int32

	mutexAvgRequestMs  sync.Mutex
	avgRequestMs       float64
	avgRequestNSamples int32
}

func (s *statInfo) incCompleteRequests() {
	atomic.AddInt32(&s.completeRequests, 1)
}
func (s *statInfo) incFailedRequests() {
	atomic.AddInt32(&s.failedRequests, 1)
}
func (s *statInfo) calAvgRequestMs(ms float64) {
	s.mutexAvgRequestMs.Lock()
	defer s.mutexAvgRequestMs.Unlock()

	s.avgRequestNSamples++

	n := float64(s.avgRequestNSamples)
	s.avgRequestMs = ((n-1)/n)*s.avgRequestMs + ms/n
}

var stat = &statInfo{completeRequests: 0,
	failedRequests:     0,
	avgRequestMs:       0.0,
	avgRequestNSamples: 0}

var (
	packSize     int
	readTimeout  int
	writeTimeout int
)

func makeRequest(conn net.Conn) error {

	var err error
	var n int
	var pos int

	// Send request
	req := pack.New(packSize)
	conn.SetWriteDeadline(time.Now().Add(time.Duration(writeTimeout) * time.Second))
	_, err = conn.Write(req.Raw())
	if err != nil {
		return err
	}

	// Read response
	resp := pack.New(packSize)
	rbuf := resp.Raw()
	pos = 0
	for pos < len(rbuf) {
		conn.SetReadDeadline(time.Now().Add(time.Duration(readTimeout) * time.Second))
		n, err = conn.Read(rbuf[pos:])
		if err != nil {
			fmt.Println(err)
			return err
		}
		pos += n
	}

	if !resp.Check() {
		fmt.Println("Package check error")
	}
	return nil
}

func worker(name string, addr string, requests int, c chan int) {

	var conn net.Conn
	var err error

	total := 0
	for retry := 0; retry < 3; retry++ {

		if retry > 0 {
			fmt.Printf("[%v] Wait interval to retry, %v\n", name, retry)
			time.Sleep(3 * time.Second)
		}

		conn, err = net.Dial("tcp", addr)
		if err != nil {
			fmt.Println(name, err)
			continue
		}

		for ; total < requests; total++ {

			startMs := float64((time.Now().UnixNano()) / 1000000)

			err = makeRequest(conn)
			if err != nil {
				stat.incFailedRequests()
				break
			}

			endMs := float64((time.Now().UnixNano()) / 1000000)
			totalMs := endMs - startMs

			stat.incCompleteRequests()
			stat.calAvgRequestMs(totalMs)
		}

		conn.Close()

		// Complete all requests, otherwise some error happened.
		if total >= requests {
			break
		}
		// else continue
	}

	//fmt.Println(name, "closed")
	c <- 0
}

func main() {

	var ip string
	var port int
	var concurency int
	var nRequest int
	var help bool
	flag.StringVar(&ip, "h", "127.0.0.1", "Server ip.")
	flag.IntVar(&port, "p", 8000, "Server port.")
	flag.IntVar(&concurency, "c", 100, "Number of mutilple request to make.")
	flag.IntVar(&nRequest, "n", 100, "Number of request to perform.")
	flag.IntVar(&packSize, "packsize", 100, "Package size.")
	flag.IntVar(&readTimeout, "rto", 5, "Read timeout.")
	flag.IntVar(&writeTimeout, "wto", 5, "Write timeout.")
	flag.BoolVar(&help, "help", false, "Help message.")
	flag.Parse()

	if help {
		flag.PrintDefaults()
		return
	}

	var addr = fmt.Sprintf("%v:%v", ip, port)
	c := make(chan int, concurency)

	fmt.Printf("Server Addr %v\n", addr)
	fmt.Printf("Concurency %v\n", concurency)
	fmt.Printf("Total Requests %v(%v*%v)\n", concurency*nRequest, concurency, nRequest)
	fmt.Println("")
	fmt.Println("Benchmarking ... wait")

	startMs := float64((time.Now().UnixNano()) / 1000000)
	for i := 0; i < concurency; i++ {
		name := fmt.Sprintf("worker_%v", i)
		go worker(name, addr, nRequest, c)
	}

	// Wait worker complete
	for i := 0; i < concurency; i++ {
		<-c
	}

	endMs := float64((time.Now().UnixNano()) / 1000000)
	totalMs := endMs - startMs

	if totalMs == 0 {
		totalMs = 1
	}

	qps := float64(stat.completeRequests) / totalMs
	tpr := float64(0.00)
	if stat.completeRequests > 0 {
		tpr = totalMs / float64(stat.completeRequests)
	}

	fmt.Printf("\n\n")
	fmt.Printf("Complete Requests:           %v\n", stat.completeRequests)
	fmt.Printf("Failed Requests:             %v\n", stat.failedRequests)
	fmt.Printf("Total Time:                  %0.2f [sec]\n", totalMs/1000)
	fmt.Printf("Requests per second:         %0.2f\n", qps)
	fmt.Printf("Time per request:            %0.2f [ms] (Across all concurency)\n", tpr)
	fmt.Printf("Time per request:            %0.2f [ms] (Average)\n", stat.avgRequestMs)
}
