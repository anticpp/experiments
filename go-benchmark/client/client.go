package main

import (
	"fmt"
	"flag"
	"net"
	"time"
	"github.com/anticpp/experiments/go-benchmark/pack"
)
var (
	packSize int
	readTimeout int
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
	if err!=nil {
		return err
	}

	// Read response
	resp := pack.New(packSize)
	rbuf := resp.Raw()
	pos = 0
	for pos < len(rbuf) {
		conn.SetReadDeadline(time.Now().Add(time.Duration(readTimeout) * time.Second))
		n, err = conn.Read(rbuf[pos:])
		if err!=nil {
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
	for retry:=0; retry<3; retry++ {

		if retry>0 {
			fmt.Printf("[%v] Wait interval to retry, %v\n", name, retry)
			time.Sleep(3 * time.Second)
		}

		conn, err = net.Dial("tcp", addr)
		if err!=nil {
			fmt.Println(name, err)
			continue
		}

		for ; total<requests; total++ {

			//fmt.Printf("[%v] %v request\n", name, total)
			err = makeRequest(conn)
			if err!=nil {
				break
			}
		}

		conn.Close()

		// Complete all requests, otherwise some error happened. 
		if total>=requests {
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

	fmt.Printf("Addr %v, concurency %v, requests %v\n", addr, concurency, nRequest)
	for i:=0; i<concurency; i++ {
		name := fmt.Sprintf("worker_%v", i)
		go worker(name, addr, nRequest, c)
	}
	fmt.Printf("%v worker spawned\n", concurency)

	// Wait worker complete
	for i:=0; i<concurency; i++ {
		<-c
	}

	fmt.Println("Finished")
}
