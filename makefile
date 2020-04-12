LINK_TARGET = main.exe

all: $(LINK_TARGET)
	

OBJS = main.o

REBUILDABLES = $(OBJS) $(LINK_TARGET)

clean:
	rm -f $(REBUILDABLES)

$(LINK_TARGET) : $(OBJS)
	g++ -g -o $@ $^

%.o : %.cpp
	g++ -g -o $@ -c $<

main.o : main.h

