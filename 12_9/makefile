.PHONY:all
all:myprocess test
test:test.cc
	g++ -o $@ $^ -std=c++11
myprocess:myprocess.c
	gcc -o $@ $^
clean:
	rm -f myprocess test

