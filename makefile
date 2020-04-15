CC := g++
CFLAGS := -Wall -g
TARGET := main

rt = $(shell pwd)

all: main

main: main.o $(rt)/src/Vec3.o $(rt)/src/Plot.o $(rt)/src/Ray.o $(rt)/src/Primitive.o $(rt)/src/Sphere.o $(rt)/src/Scene.o
	g++ -o main main.o $(rt)/src/Vec3.o $(rt)/src/Plot.o $(rt)/src/Ray.o $(rt)/src/Primitive.o $(rt)/src/Sphere.o $(rt)/src/Scene.o

main.o:
	g++ -c main.cpp

/src/Vec3.o:
	g++ -c $(rt)/src/Vec3.cpp

/src/Plot.o:
	g++ -c $(rt)/src/Plot.cpp

/src/Ray.o:
	g++ -c $(rt)/src/Ray.cpp

/src/Primitive.o:
	g++ -c $(rt)/src/Primitive.cpp

/src/Sphere.o:
	g++ -c $(rt)/src/Sphere.cpp

/src/Scene.o:
	g++ -c $(rt)/src/Scene.cpp

clean:
	rm -rf main.o $(rt)/src/Vec3.o $(rt)/src/Plot.o $(rt)/src/Ray.o $(rt)/src/Primitive.o $(rt)/src/Sphere.o $(rt)/src/Sphere.o $(rt)/src/Scene.o main  
	
.PHONY: all clean
