
.PHONY: all clean

SRCS=$(wildcard *.c)

sort: $(SRCS)
	gcc main.c -o $@

all: sort

clean:
	rm -vf sort *.o
