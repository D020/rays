CC := g++
CFLAGS := -Wall -g
TARGET := main

rt = $(shell pwd)

all: main

main: main.o $(rt)/src/Vec3.o
	g++ -o main main.o $(rt)/src/Vec3.o

main.o:
	g++ -c main.cpp

/src/Vec3.o:
	g++ -c $(rt)/src/Vec3.cpp

clean:
	rm -rf $(TARGET) *.o
	
.PHONY: all clean
