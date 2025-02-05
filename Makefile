CC = gcc
CFLAGS = -Wall -Wextra -Wswitch-enum -std=c11 -pedantic
LIBS =

all: rasm rvme derasm

.PHONY: all

rasm: ./src/rasm.c ./src/rvm.c ./src/rvm.h
	$(CC) $(CFLAGS) -o rasm ./src/rasm.c ./src/rvm.c $(LIBS)
derasm: ./src/derasm.c ./src/rvm.c ./src/rvm.h
	$(CC) $(CFLAGS) -o derasm ./src/derasm.c ./src/rvm.c $(LIBS)

rvme: ./src/rvme.c ./src/rvm.c ./src/rvm.h
	$(CC) $(CFLAGS) -o rvme ./src/rvme.c ./src/rvm.c $(LIBS)
.PHONY: examples

examples: ./examples/fib.rvm ./examples/123.rvm

./examples/fib.rvm: rasm ./examples/fib.rasm
	./rasm ./examples/fib.rasm ./examples/fib.rvm

./examples/123.rvm: rasm ./examples/123.rasm
	./rasm ./examples/123.rasm ./examples/123.rvm
