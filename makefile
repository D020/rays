CC := g++
CFLAGS := -Wall -g
TARGET := main

rt = $(shell pwd)

all: main

main: main.o $(rt)/src/Vec3.o $(rt)/src/Plot.o $(rt)/src/Ray.o
	g++ -o main main.o $(rt)/src/Vec3.o $(rt)/src/Plot.o $(rt)/src/Ray.o

main.o:
	g++ -c main.cpp

/src/Vec3.o:
	g++ -c $(rt)/src/Vec3.cpp

/src/Plot.o:
	g++ -c $(rt)/src/Plot.cpp

/src/Ray.o:
	g++ -c $(rt)/src/Ray.cpp

clean:
	rm -rf main.o $(rt)/src/Vec3.o $(rt)/src/Plot.o $(rt)/src/Ray.o
	
.PHONY: all clean
