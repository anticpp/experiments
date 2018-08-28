
.PHONY: all clean

test: test.c stack.c queue.c rpn.c
	gcc test.c -o $@

calc: calc.c stack.c queue.c rpn.c
	gcc calc.c -o $@

all: test calc

clean:
	rm -vf test calc
