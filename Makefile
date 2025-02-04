CC = gcc
CFLAGS = -Wall -Wextra -Wswitch-enum -std=c11 -pedantic
LIBS =

all: rasm rvmi

.PHONY: all

rasm: ./src/rasm.c ./src/rvm.c
	$(CC) $(CFLAGS) -o rasm ./src/rasm.c $(LIBS)

rvmi: ./src/rvmi.c ./src/rvm.c
	$(CC) $(CFLAGS) -o rvmi ./src/rvmi.c $(LIBS)
.PHONY: examples

examples: ./examples/fib.rvm ./examples/123.rvm

./examples/fib.rvm: ./examples/fib.rasm
	./rasm ./examples/fib.rasm ./examples/fib.rvm

./examples/123.rvm: ./examples/123.rasm
	./rasm ./examples/123.rasm ./examples/123.rvm
