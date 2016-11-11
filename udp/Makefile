
all: client server

CFLAGS=-std=c99

client: client.o
	$(GCC) $^ -o $@

server: server.o
	$(GCC) $^ -o $@

.PHONY: all

clean:
	rm -rf client server *.o


GCC=gcc
