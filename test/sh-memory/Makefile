CC=/usr/lib/gcc-astra/bin/gcc

FULL_FLAGS= -Wall -Wextra -pedantic -O2 -Wshadow -Wformat=2 \
 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 \
 -Wduplicated-cond -Wcast-qual -Wcast-align -D_GLIBCXX_DEBUG \
 -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 \
 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover \
 -fstack-protector


X1FLAGS= -Wshadow -Wfloat-equal -Wconversion -Wduplicated-cond -Wlogical-op
X2FLAGS= -Wshift-overflow=2 -Wcast-qual -Wcast-align -fstack-protector
X3FLAGS= -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2
X4FLAGS= -fsanitize=address -fsanitize=undefined -fno-sanitize-recover 

CCVER= -std=c++20
#-std=c++11

CFLAGS= -c -MD -MP -Wall -Wextra -Wfatal-errors -pedantic -O2 
CFLAGS+= $(CCVER)
CFLAGS+= $(X1FLAGS)
CFLAGS+= $(X2FLAGS)
CFLAGS+= $(X3FLAGS)
#CFLAGS+= $(X4FLAGS)

LDFLAGS= -Wall -Wextra -pedantic -O2
LDFLAGS+= $(X1FLAGS)
LDFLAGS+= $(X2FLAGS)
LDFLAGS+= $(X3FLAGS)
#LDFLAGS+= $(X4FLAGS)

#-Wl,–verbose

SOURCES=plc32.cpp plc_read.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=hello
LIBS= -lrt
#LIBS= -lconfig -lmodbus
#INCLUDES = -I/usr/include/modbus

all: $(patsubst %.c,%.o,$(wildcard *.c))
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)
#       gcc $^ -o $@

sh_mem: $(patsubst %.c,%.o,$(wildcard *.c))
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)


%.o: %.c
	$(CC) $(CFLAGS) $<
#	$(CC) $(CFLAGS) $<
#       gcc -c -MD $<

include $(wildcard *.d)


$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@


clean:
	rm -rf *.o
	rm -rf *.d
	rm -rf plc
