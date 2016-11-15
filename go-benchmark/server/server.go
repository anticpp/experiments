package main

import (
	"fmt"
	"net"
	"flag"
)

var (
	incomeChanSize = 10000
	outgoingChanSize = 10000
	packSize = 4
)

type pack struct {
	buf []byte
}
func newPack() *pack {
	p := &pack{buf: make([]byte, packSize)}
	return p
}

type session struct {
	conn net.Conn
	stop bool

	income chan *pack
	outgoing chan *pack

	closeSignal chan int
}

func newSession(conn net.Conn) *session {
	return &session{
			conn: conn,
			stop: false,
			income: make(chan *pack, incomeChanSize),
			outgoing: make(chan *pack, outgoingChanSize),
			closeSignal: make(chan int),
	}
}

func (s *session) setStop() {
	s.stop = true
	close(s.income)
	close(s.outgoing)
}
func (s *session) isNormal() bool {
	return s.stop
}
func (s *session) sendIncome(p *pack) {
	s.income <- p
}
func (s *session) sendOutgoing(p *pack) {
	s.outgoing <- p
}
func (s *session) start() {
	go s.serve_read()
	go s.serve_write()
	go s.serve_pack()
}
func (s *session) serve_read() {

	pack := newPack()

	var n int
	var err error
	for s.isNormal() {

		pos := 0
		for pos < len(pack.buf) {
			n, err = s.conn.Read(pack.buf[pos:])
			if err!=nil {
				s.setStop()
				break;
			}
			pos += n
		}

		s.sendIncome(pack)
	}

	// Wait serve_write, server_pack
	<-s.closeSignal
	<-s.closeSignal

	s.conn.Close()
}
func (s *session) serve_write() {

	for {

		p, ok := <-s.outgoing
		if !ok {
			break
		}

		_, err := s.conn.Write(p.buf)
		if err!=nil {
			s.setStop()
			break
		}

	}

	s.closeSignal <- 0
}
func (s *session) serve_pack() {

	for {

		p, ok := <-s.income
		if !ok {
			break
		}

		// Doing some job
		// ...

		s.sendOutgoing(p)

	}

	s.closeSignal <- 0
}

func main() {

	var ip = flag.String("h", "", "Listen ip. Default INADDR_ANY.")
	var port = flag.Int("p", 8000, "Listen port. Default 8000.")
	var help = flag.Bool("help", false, "Help message.")
	flag.Parse();

	if *help {
		flag.PrintDefaults()
		return
	}

	var addr = fmt.Sprintf("%v:%v", *ip, *port)
	fmt.Printf("Listen on %v\n", addr)
	return

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
		s := newSession(conn)
		s.start()

	}
}
