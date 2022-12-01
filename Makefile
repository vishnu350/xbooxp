## Simple Linux makefile to build xbooxp

all:
	g++ main.cpp -o xbooxp

clean:
	rm -rf xbooxp
