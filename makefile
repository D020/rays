GCC = g++
#GCC = gcc
#ALL_SOURCES = $(shell find . -name '*.c') # all .c files in this dir or any subdirs
#PROG_SOURCES = $(filter-out test%, $(ALL_SOURCES))
#PROG_SOURCES = $(filter-out $(wildcard *test*), $(ALL_SOURCES))

SOURCES = $(shell find . -type f -name '*.cpp' -a ! -name '*test*')

OBJS := $(patsubst %.c,%.o,$(SOURCES))

#Debug build
#CFLAGS = -g -Wall

#Optimized build
CFLAGS = -O2 -flto -Wall 

LIBS := pthread
LIBS := $(addprefix -l,$(LIBS))

all: main

%.o: %.c
	$(GCC) $(CFLAGS) -c -o $@ $<

main: $(OBJS)
	$(GCC) $(OBJS) $(LIBS) $(CFLAGS) -o main

clean:
	find . -name "*.o" -type f -delete
	find . -name "*.ppm" -type f -delete
	rm -rf main
