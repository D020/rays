GCC = g++
#GCC = gcc
#ALL_SOURCES = $(shell find . -name '*.c') # all .c files in this dir or any subdirs
#PROG_SOURCES = $(filter-out test%, $(ALL_SOURCES))
#PROG_SOURCES = $(filter-out $(wildcard *test*), $(ALL_SOURCES))

SOURCES = $(shell find . -type f -name '*.cpp' -a ! -name '*test*')

OBJS := $(patsubst %.c,%.o,$(SOURCES))
CFLAGS = -g -Wall

LIBS := pthread
LIBS := $(addprefix -l,$(LIBS))

all: main

%.o: %.c
	$(GCC) $(CFLAGS) -c -o $@ $<

main: $(OBJS)
	$(GCC) $(OBJS) $(LIBS) $(CFLAGS) -o main

clean:
	rm -rf main.o $(rt)/src/Vec3.o $(rt)/src/Plot.o $(rt)/src/Ray.o $(rt)/src/Primitive.o $(rt)/src/Sphere.o $(rt)/src/Sphere.o $(rt)/src/Scene.o main
