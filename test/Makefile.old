CC=g++
#CC=/usr/lib/gcc-astra/bin/g++

.DEFAULT_GOAL := all
CPP_VER= -std=c++17

# === Directories & files
OUTFILE=a.out

OBJDIR=./tmp/obj
$(shell mkdir $(OBJDIR) &2>/dev/null)

SRCDIRS:=. include sources
$(foreach dir,$(SRCDIRS),$(shell mkdir $(OBJDIR)/$(dir) &2>/dev/null))

SRCFILES=$(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.cpp))
OBJLIST =$(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCFILES)) 
OBJFILES=$(foreach dir,$(SRCDIRS),$(wildcard $(OBJDIR)/$(dir)/*.o))
DEPFILES=$(foreach dir,$(SRCDIRS),$(wildcard $(OBJDIR)/$(dir)/*.cpp.d))

ASTYLEFILES=$(foreach dir,$(SRCDIRS),$(dir)/*.cpp,*.h)
CLANGFILES =$(foreach dir,$(SRCDIRS),$(dir)/*.cpp $(dir)/*.h)

# $(patsubst %.cpp,$(OBJDIR)/%.cpp.d,$(SRCLIST))
#DEPLIST= $(wildcard $(OBJDIR)/**/*.cpp.d)

SRCDIR1=.
SRCDIR2=include
SRCDIR3=sources
##SRCDIR1=./

# === Add libs here 
LIBS= -lrt -lpthread
LIBCONFIG=$(shell pkg-config libconfig++ --libs)
LIBMODBUS=$(shell pkg-config --libs --cflags libmodbus)
#LIBNCURSES=$(shell pkg-config ncurses --libs)

LIBS+= $(LIBCONFIG) $(LIBMODBUS) 
# $(LIBNCURSES)

#LIBS= -lconfig -lmodbus
#INCLUDES = -I/usr/include/modbus
#SOURCES=plc32.cpp plc_read.cpp
#OBJECTS=$(SOURCES:.cpp=.o)
#EXECUTABLE=hello

# === Colors (just for fun)
RED='\033[0;91m'
GRE="\033[0;32m"
GRB="\033[0;92m"
YEL="\033[0;93m"
YEB="\033[1;33m"
BLU='\033[0;94m'
WHI='\033[0;97m'
NC='\033[0m' # No Color

# === C/CPP flags configuretion ===

LDFLAGS= -Wall $(CPP_VER)
CFLAGS= -c -Wall $(CPP_VER)
DEPFLAGS= -MD -MF
OPTFLAGS= -flto=auto -O2

ASFLAGS= -k1 -W3 -xg -xb -xj -xp -c -O -H

# === Slote here https://codeforces.com/blog/entry/15547

WARN1_FLAGS= -Wextra -Wfatal-errors -pedantic -Wformat=2 -fconcepts
WARN2_FLAGS= -Wshadow -Wfloat-equal -Wconversion -Wduplicated-cond
WARN3_FLAGS= -Wshift-overflow=2 -Wcast-qual -Wcast-align -Wlogical-op
GLIBC_FLAGS= -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fstack-protector
SANIT_FLAGS= -fsanitize=address -fsanitize=undefined -fno-sanitize-recover
DEBUG_FLAGS= -g -DDEBUG_FLAG

CFLAGS+= $(WARN1_FLAGS)
CFLAGS+= $(WARN2_FLAGS)
CFLAGS+= $(WARN3_FLAGS)
#CFLAGS+= $(GLIBC_FLAGS)
#CFLAGS+= $(SANIT_FLAGS)
#CFLAGS+= -fanalyzer


# === Check for DEBUG build ===

$(info === The GOALS is: $(MAKECMDGOALS))

ifeq ("master","$(filter master,$(MAKECMDGOALS))")
OPTFLAGS=
CFLAGS+= -DMB_MASTER
MESSAGE=" MASTER"
OUTFILE=mb_master
$(info === MASTER mode activated! ===)
endif

ifeq ("slave","$(filter slave,$(MAKECMDGOALS))")
OPTFLAGS=
CFLAGS+= -DMB_SLAVE
MESSAGE=" SLAVE"
OUTFILE=mb_slave
$(info === SLAVE mode activated! ===)
endif


ifeq ("check","$(filter check,$(MAKECMDGOALS))")
OPTFLAGS=
CFLAGS+= $(GLIBC_FLAGS)
CFLAGS+= $(SANIT_FLAGS)
LDFLAGS+= $(GLIBC_FLAGS)
LDFLAGS+= $(SANIT_FLAGS)
MESSAGE=" with CHECK"
$(info === FULL CHECK options activated! ===)
endif

ifeq ("debug","$(filter debug,$(MAKECMDGOALS))")
OPTFLAGS=
CFLAGS+= $(DEBUG_FLAGS)
LDFLAGS+= $(DEBUG_FLAGS)
DO_DEBUG=YES
MESSAGE=" with DEBUG"
$(info === DEDUG options activated! ===)
endif

ifeq ("fulldebug","$(filter fulldebug,$(MAKECMDGOALS))")
OPTFLAGS=
CFLAGS+= $(GLIBC_FLAGS)
CFLAGS+= $(SANIT_FLAGS)
CFLAGS+= $(DEBUG_FLAGS)
LDFLAGS+= $(SANIT_FLAGS)
LDFLAGS+= $(DEBUG_FLAGS)
DO_DEBUG=YES
MESSAGE=" FULL DEBUG!"
$(info === FULL Debug options activated! ===)
endif

# =====================================================================

FULL_FLAGS= -Wall -Wextra -pedantic -O2 -Wshadow -Wformat=2 \
 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 \
 -Wduplicated-cond -Wcast-qual -Wcast-align -D_GLIBCXX_DEBUG \
 -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 \
 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover \
 -fstack-protector

#======================================================================

SRCFORMAT1="./*.cpp,*.h"
SRCFORMAT2="$(SRCDIR2)/*.cpp,*.h"
SRCFORMAT3="$(SRCDIR3)/*.cpp,*.h"

ASFILES=

CFFILES=

#SRCLIST1= $(wildcard $(SRCDIR1)/*.cpp)
#SRCLIST2= $(wildcard $(SRCDIR2)/*.cpp)
#SRCLIST3= $(wildcard $(SRCDIR3)/*.cpp)
#SRCLIST= $(SRCLIST1) $(SRCLIST2) $(SRCLIST3)

#SRCFILES= $(wildcard $(SRCDIR1)/*.cpp $(SRCDIR2)/*.cpp $(SRCDIR3)/*.cpp)

#OBJLIST1= $(patsubst %.cpp,$(OBJDIR)/%.o,$(wildcard $(SRCDIR1)/*.cpp))
#OBJLIST2= $(patsubst %.cpp,$(OBJDIR)/%.o,$(wildcard $(SRCDIR2)/*.cpp)) 
#OBJLIST3= $(patsubst %.cpp,$(OBJDIR)/%.o,$(wildcard $(SRCDIR3)/*.cpp)) 
#OBJLIST= $(OBJLIST1) $(OBJLIST2) $(OBJLIST3)

#OBJLIST= $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCLIST)) 
#OBJLISTNEW= $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCFILES)) 

#DEPLIST1= $(wildcard $(OBJDIR)/$(SRCDIR1)/*.cpp.d)
#DEPLIST2= $(wildcard $(OBJDIR)/$(SRCDIR2)/*.cpp.d)
#DEPLIST3= $(wildcard $(OBJDIR)/$(SRCDIR3)/*.cpp.d)
#DEPLIST= $(DEPLIST1) $(DEPLIST2) $(DEPLIST3)

#DEPLIST= $(wildcard $(OBJDIR)/**/*.cpp.d)
#DEPFILES= $(patsubst %.cpp,$(OBJDIR)/%.cpp.d,$(SRCLIST))

include $(DEPFILES)

#include $(wildcard $(OBJDIR)/*.cpp.d)

# =============================================
all: master

master: a.out

slave: a.out

run: clean a.out

check: clean a.out

debug: clean a.out

fulldebug: clean a.out

# ================ Linking ================================
#a.out: #$(patsubst %.cpp,$(OBJDIR)/%.o,$(wildcard *.cpp)) $(patsubst %.cpp,$(OBJDIR)/%.o,$(wildcard include/*.cpp))

a.out: $(OBJLIST)
	@echo -e $(GRE)"=== Linking$(MESSAGE): $@"$(NC)
	$(CC) $(LDFLAGS) $(OPTFLAGS) $^ -o ./tmp/$@ $(LIBS)
	@echo -e $(GRE)"=== Finished$(MESSAGE)==="$(NC)
	cp --force ./tmp/$@ $(OUTFILE)
	@ls -Fog --color $(OUTFILE)
	@echo -e $(GRE)"=== Done$(MESSAGE)===" $(NC)
	sleep 2
ifdef DO_DEBUG
	@echo -e $(GRE)"=== The size of executable file are REALLY BIG. ==="$(NC)
	sleep 2
endif

#================== Compiling ==============================
$(OBJDIR)/%.o: %.cpp
	@echo -e $(YEL)"=== Compiling$(MESSAGE): $<"$(NC)
	$(CC) $(CFLAGS) $(OPTFLAGS) $(DEPFLAGS) $(OBJDIR)/$<.d -o $@ $<
#	$(CC) -dumpdir obj/ $(CFLAGS) $(DEPFLAGS) ./obj/$<.d $<
#       gcc -c -MD $<

# ================== Cleaning =============================
clean: format-linux
	@echo -e $(BLU)"=== ASTYLE list-FULL..."$(NC)
	@echo $(ASTYLEFILES)

	@echo -e $(BLU)"=== CLANG list-FULL..."$(NC)
	@echo $(CLANGFILES)

	@echo -e $(BLU)"=== Src-FILES list-FULL..."$(NC)
	@echo $(SRCFILES)


#	@echo -e $(BLU)"=== Obj list-1..."$(NC)
#	@echo $(OBJLIST1)
#	@echo -e $(BLU)"=== Obj list-2..."$(NC)
#	@echo $(OBJLIST2)
#	@echo -e $(BLU)"=== Obj list-2..."$(NC)
#	@echo $(OBJLIST3)

	@echo -e $(BLU)"=== Obj list-FULL..."$(NC)
	@echo $(OBJLIST)

#	@echo -e $(BLU)"=== Obj-NEW list-FULL..."$(NC)
#	@echo $(OBJLISTNEW)

#	@echo -e $(BLU)"=== Depencies list..."$(NC)
#	@echo $(DEPLIST)

	@echo -e $(BLU)"=== Dep-FILES list..."$(NC)
	@echo $(DEPFILES)

	@echo -e $(BLU)"=== Cleaning UP..."$(NC)
	@rm -rfv $(OBJFILES) $(DEPFILES)
#	rm -rfv $(OBJDIR)/*.o $(OBJDIR)/*.d
#	rm -rfv $(OBJDIR)/$(SRCDIR2)/*.o $(OBJDIR)/$(SRCDIR2)/*.d
#	rm -rfv $(OBJDIR)/$(SRCDIR3)/*.o $(OBJDIR)/$(SRCDIR3)/*.d
	rm -rfv a.out

# ================== Formatting ===========================
# Simple format current directory only
clang:
	clang-format -i --verbose *.cpp *.h

google:
	clang-format -i -style=google --verbose *.cpp *.h

# ================ ALL FILE recursively! ==================
# Reindent *.cpp to Linux code-style
format-linux:
	astyle $(ASFLAGS) -n -s2 --style=linux $(ASTYLEFILES)
#	astyle $(ASFLAGS) -n -s2 --style=linux $(SRCFORMAT1), $(SRCFORMAT2), $(SRCFORMAT3)
#"./*.cpp,*.h", "include/*.cpp,*.h"

# Reindent *.cpp to K&R code-style
format-kr:
	astyle $(ASFLAGS) -n --style=kr $(ASTYLEFILES)
#$(SRCFORMAT1), $(SRCFORMAT2), $(SRCFORMAT3)

# Reindent *.cpp to Allman code-style
format-allman:
	astyle $(ASFLAGS) -n --style=allman $(ASTYLEFILES)
#$(SRCFORMAT1), $(SRCFORMAT2), $(SRCFORMAT3)

# Reindent *.cpp to Google code-style
format-google2:
	astyle $(ASFLAGS) -n -s2 --style=google $(ASTYLEFILES)
#$(SRCFORMAT1), $(SRCFORMAT2), $(SRCFORMAT3)

# Reindent *.cpp to LLVM code-style
format-clang:
	clang-format -i --verbose $(CLANGFILES)
#	clang-format -i --verbose *.cpp *.h $(SRCDIR2)/*.cpp $(SRCDIR2)/*.h $(SRCDIR3)/*.cpp $(SRCDIR3)/*.h

# Reindent *.cpp to Google code-style
format-google:
	clang-format -i -style=google --verbose $(CLANGFILES)
#	clang-format -i -style=google --verbose *.cpp *.h $(SRCDIR2)/*.cpp $(SRCDIR2)/*.h $(SRCDIR3)/*.cpp $(SRCDIR3)/*.h


# =======================================
#$(EXECUTABLE): $(OBJECTS)
#	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
