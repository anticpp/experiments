
.PHONY: all clean

sort: main.c array.c count.c isort.c qsort.c test_data.c
	gcc main.c -o $@

all: sort

clean:
	rm -vf sort *.o
