CC=g++
#CC=/usr/lib/gcc-astra/bin/g++



.DEFAULT_GOAL := all

OBJDIR=./obj

LIBS= -lrt
LIBCONFIG=$(shell pkg-config libconfig++ --libs)
LIBNIDBUS=$(shell pkg-config --libs --cflags libmodbus)
LIBS+= $(LIBCONFIG) $(LIBMODBUS) 

#LIBS= -lconfig -lmodbus
#INCLUDES = -I/usr/include/modbus
#SOURCES=plc32.cpp plc_read.cpp
#OBJECTS=$(SOURCES:.cpp=.o)
#EXECUTABLE=hello


# === C/CPP flags configuretion ===

LDFLAGS= -Wall
CFLAGS= -c -Wall
DEPFLAGS= -MD -MF

CPP_VER= -std=c++17
WARN_FLAGS=  -Wextra -Wfatal-errors -pedantic -O2 
CHECK_FLAGS= -Wshadow -Wfloat-equal -Wconversion -Wduplicated-cond -Wlogical-op
TYPES_FLAGS= -Wshift-overflow=2 -Wcast-qual -Wcast-align -fstack-protector
GLIBC_FLAGS= -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2
DEBUG_FLAGS= -g -fsanitize=address -fsanitize=undefined -fno-sanitize-recover 

CFLAGS+= $(CPP_VER)
CFLAGS+= $(WARN_FLAGS)
CFLAGS+= $(CHECK_FLAGS)
CFLAGS+= $(TYPES_FLAGS)
CFLAGS+= $(GLIBC_FLAGS)

# === Check for DEBUG build ===

$(info === The GOALS is: $(MAKECMDGOALS))

ifeq ("debug","$(filter debug,$(MAKECMDGOALS))")
CFLAGS+= $(DEBUG_FLAGS)
LDFLAGS+= $(DEBUG_FLAGS)
DO_DEBUG=YES
$(info === Debug options activated! ===)
endif

# =============================

FULL_FLAGS= -Wall -Wextra -pedantic -O2 -Wshadow -Wformat=2 \
 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 \
 -Wduplicated-cond -Wcast-qual -Wcast-align -D_GLIBCXX_DEBUG \
 -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 \
 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover \
 -fstack-protector

#===============================================================================

all: clean a.out

debug: clean a.out

libtest: a.out

a.out: $(patsubst %.cpp,$(OBJDIR)/%.o,$(wildcard *.cpp))
ifdef DO_DEBUG
	@echo "=== Linking with DEBUG: $@"
else
	@echo "=== Linking: $@"
endif
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)
ifdef DO_DEBUG
	@echo "==="
	@echo "=== Finished with DEBUG! ==="
	@echo "=== The size of executable file will be REALLY BIG. ==="
	@echo "==="
else
	@echo "=== Done. ==="
endif


$(OBJDIR)/%.o: %.cpp
ifeq ("YES","$(DO_DEBUG)")
	@echo "=== Compiling with DEBUG: $<"
else
	@echo "=== Compiling: $<"
endif
	$(CC) $(CFLAGS) $(DEPFLAGS) $(OBJDIR)/$<.d -o $@ $<
#	$(CC) -dumpdir obj/ $(CFLAGS) $(DEPFLAGS) ./obj/$<.d $<
#       gcc -c -MD $<

include $(wildcard $(OBJDIR)/*.cpp.d)


clean:
	@echo "=== Cleaning UP..."
	rm -rfv $(OBJDIR)/*.o $(OBJDIR)/*.d
	rm -rfv a.out

# =======================================
#$(EXECUTABLE): $(OBJECTS)
#	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
