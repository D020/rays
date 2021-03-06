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
#CFLAGS = -O2 -flto -Wall

#Optmized bug-fix build
CFLAGS = -O2 -flto -Wall -Wl,--no-as-needed

LIBS := pthread
LIBS := $(addprefix -,$(LIBS))

all: main

%.o: %.c
	$(GCC) $(CFLAGS) -c -o $@ $<

main: $(OBJS)
	$(GCC) $(OBJS) $(CFLAGS) $(LIBS) -o main

clean:
	find . -name "*.o" -type f -delete
	rm -rf main

linecount:
	find . -name '*.cpp' -not -name "*test*" | xargs wc -l

	
#find . -name "*.ppm" -type f -delete
