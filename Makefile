.PHONY: compile clean
.DEFAULT_GOAL := compile

CC = g++
FLAGS = -std=c++11 -I/usr/include/python2.7 -lpython2.7 -O0 -DWITHOUT_NUMPY

compile:
	${CC} $(wildcard *.cpp) ${FLAGS}

clean:
	rm a.out
