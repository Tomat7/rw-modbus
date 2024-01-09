CC=g++
CFLAGS=-c -std=c++20 -MD -MP
XXFLAGS= -Wall -Wextra -pedantic -O2 -Wshadow -Wformat=2 \
 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 \
 -Wduplicated-cond -Wcast-qual -Wcast-align -D_GLIBCXX_DEBUG \
 -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 \
 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover \
 -fstack-protector
LDFLAGS=
#-Wl,–s
SOURCES=plc32.cpp plc_read.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=hello
LIBS= -lconfig -lmodbus
#INCLUDES = -I/usr/include/modbus

plc: $(patsubst %.cpp,%.o,$(wildcard *.cpp))
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)
#	gcc $^ -o $@ 

%.o: %.cpp
	$(CC) $(CFLAGS) $<
#	gcc -c -MD $<

include $(wildcard *.d) 


$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@


clean:
	rm -rf *.o
	rm -rf *.d
	rm -rf plc

