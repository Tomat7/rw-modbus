CC=gcc
CFLAGS=-c -Wall -MD -MP
LDFLAGS=
SOURCES=plc32.cpp plc_read.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=hello
LIBS= -lconfig -lmodbus
#INCLUDES = -I/usr/include/modbus

plc: $(patsubst %.c,%.o,$(wildcard *.c))
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)
#	gcc $^ -o $@ 

%.o: %.c
	$(CC) $(CFLAGS) $<
#	gcc -c -MD $<

include $(wildcard *.d) 


$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@


clean:
	rm -rf *.o
	rm -rf *.d
	rm -rf plc

