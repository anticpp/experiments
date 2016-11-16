package main

import (
	"fmt"
	"net"
	"time"
	"flag"
	"github.com/anticpp/experiments/go-benchmark/pack"
)

var (
	incomeChanSize int
	outgoingChanSize int
	packSize int
)

type session struct {
	conn net.Conn
	stop bool

	income chan *pack.Pack
	outgoing chan *pack.Pack

	closeSignal chan int
}

func newSession(conn net.Conn) *session {
	return &session{
			conn: conn,
			stop: false,
			income: make(chan *pack.Pack, incomeChanSize),
			outgoing: make(chan *pack.Pack, outgoingChanSize),
			closeSignal: make(chan int),
	}
}

func (s *session) setStop() {
	s.stop = true
	//close(s.income)
	//close(s.outgoing)
}
func (s *session) isNormal() bool {
	return s.stop==false
}
func (s *session) sendIncome(p *pack.Pack) {
	s.income <- p
}
func (s *session) sendOutgoing(p *pack.Pack) {
	s.outgoing <- p
}
func (s *session) start() {
	go s.serve_read()
	go s.serve_write()
	go s.serve_pack()
}
func (s *session) serve_read() {

	var n int
	var err error
	for s.isNormal() {

		req := pack.New(packSize)
		buf := req.Raw()
		pos := 0
		for pos < len(buf) {
			n, err = s.conn.Read(buf[pos:])
			if err!=nil {
				break
			}
			pos += n
		}

		if err!=nil {
			//fmt.Println("Read error.", err)
			s.setStop()
			break;
		}

		// Check package
		if !req.Check() {
			fmt.Println("Package check error")
			s.setStop()
			break;
		}

		s.sendIncome(req)
	}

	// Wait serve_write, server_pack
	<-s.closeSignal
	<-s.closeSignal

	//fmt.Println("Close connection", s.conn.RemoteAddr())
	s.conn.Close()
}
func (s *session) serve_write() {

	for s.isNormal() {

		select {

		case p := <-s.outgoing:

			_, err := s.conn.Write(p.Raw())
			if err!=nil {
				s.setStop()
			}
			break

		case <-time.After(time.Second * 1):
			break

		}

	}

	s.closeSignal <- 0
}
func (s *session) serve_pack() {

	for s.isNormal() {

		select {

		case p := <-s.income:
			s.sendOutgoing(p)
			break

		case <-time.After(time.Second * 1):
			break

		}

	}

	s.closeSignal <- 0
}

func main() {

	var ip string
	var port int
	var help bool
	flag.StringVar(&ip, "h", "", "Listen ip.")
	flag.IntVar(&port, "p", 8000, "Listen port.")
	flag.BoolVar(&help, "help", false, "Help message.")
	flag.IntVar(&incomeChanSize, "ichan", 10000, "In channel size.")
	flag.IntVar(&outgoingChanSize, "ochan", 10000, "Out channel size.")
	flag.IntVar(&packSize, "packsize", 100, "Package size.")
	flag.Parse();

	if help {
		flag.PrintDefaults()
		return
	}

	var addr = fmt.Sprintf("%v:%v", ip, port)
	fmt.Printf("Listen on %v\n", addr)

	ln, err := net.Listen("tcp", addr)
	if err!=nil {
		panic(err)
	}
	for {

		conn, err := ln.Accept()
		if err!=nil {
			fmt.Println(err)
			continue
		}

		//fmt.Println("New connection", conn.RemoteAddr())
		s := newSession(conn)
		s.start()

	}
}
