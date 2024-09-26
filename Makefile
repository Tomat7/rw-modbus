# https://microsin.net/programming/arm/learning-makefile-with-simple-examples.html
# https://runebook.dev/ru/docs/gnu_make/foreach-function
# https://habr.com/ru/articles/534304/
# https://wiki.gentoo.org/wiki/GCC_optimization
# https://microsin.net/programming/arm/install-linux-in-microsoft-windows10-via-wsl.html
#
# To view defaults (implicit variables): make -p
# or https://runebook.dev/ru/docs/gnu_make/-index-
# Default:
# COMPILE.cpp = $(COMPILE.cc)
# COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
# LINK.o = $(CC) $(LDFLAGS) $(TARGET_ARCH)

.DEFAULT_GOAL := all
$(info === The GOALS is: $(MAKECMDGOALS))

# CXX = g++
# for using with LINK.o
CC=$(CXX)
CXX_VER= -std=c++17

# === Directories & files & libraries===
OUTFILE=a.out
OBJDIR =./tmp/obj
SUBDIRS= include sources

ALLDIRS= $(foreach dir,$(SUBDIRS),$(shell find -L $(dir) -maxdepth 1 -type d))
SRCDIRS=. $(ALLDIRS)

$(shell mkdir $(OBJDIR) 2>/dev/null)
$(foreach dir,$(SRCDIRS),$(shell mkdir $(OBJDIR)/$(dir) 2>/dev/null))

SRCFILES=$(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.cpp))
DEPFILES=$(foreach dir,$(SRCDIRS),$(wildcard $(OBJDIR)/$(dir)/*.cpp.d))

OBJLIST =$(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCFILES)) 
OBJFILES=$(foreach dir,$(SRCDIRS),$(wildcard $(OBJDIR)/$(dir)/*.o))

ASTYLEFILES=$(foreach dir,$(SRCDIRS),$(dir)/*.cpp,*.h)
CLANGFILES =$(foreach dir,$(SRCDIRS),$(dir)/*.cpp $(dir)/*.h)

OUTF=$(shell ls -Fog $(OUTFILE))

# === Add libs here ===
INCLUDES = -I.
LIBS=libmodbus libconfig++
LDLIBS= -lrt -lpthread
LDLIBS+=$(foreach lib,$(LIBS),$(shell pkg-config --libs --cflags $(lib)))

#LIBCONFIG=$(shell pkg-config --libs libconfig++)
#LIBMODBUS=$(shell pkg-config --libs --cflags libmodbus)
#LIBNCURSES=$(shell pkg-config ncurses --libs)
#LDLIBS+= $(LIBCONFIG) $(LIBMODBUS) $(LIBNCURSES)
#LIBS= -lconfig -lmodbus
#INCLUDES = -I.
#SOURCES=plc32.cpp plc_read.cpp
#OBJECTS=$(SOURCES:.cpp=.o)
#EXECUTABLE=hello

# === C/CPP flags configuretion ===
CXXFLAGS= -Wall $(CXX_VER)
LDFLAGS = -Wall $(CXX_VER)
DEPFLAGS= -MD -MF $(OBJDIR)
OPTFLAGS= -flto=auto -O2

ASTYLEFLAGS= -k1 -W3 -xg -xb -xj -xp -c -O -H

# === Stolen here https://codeforces.com/blog/entry/15547
WARN1_FLAGS= -Wextra -Wfatal-errors -pedantic -Wformat=2 -fconcepts
WARN2_FLAGS= -Wshadow -Wfloat-equal -Wconversion -Wduplicated-cond
WARN3_FLAGS= -Wshift-overflow=2 -Wcast-qual -Wcast-align -Wlogical-op
GLIBC_FLAGS= -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 
SANIT_FLAGS= -fstack-protector -fsanitize=address -fsanitize=undefined -fno-sanitize-recover
DEBUG_FLAGS= -g -DDEBUG_FLAG

CXXFLAGS+= $(WARN1_FLAGS)
CXXFLAGS+= $(WARN2_FLAGS)
CXXFLAGS+= $(WARN3_FLAGS)
#CXXFLAGS+= $(GLIBC_FLAGS)
#CXXFLAGS+= $(SANIT_FLAGS)
#CXXFLAGS+= -fanalyzer

# === Check for DEBUG build ===
MESSAGE_DEBUG="==="

ifeq ("master","$(filter master,$(MAKECMDGOALS))")
CPPFLAGS+= -DMB_MASTER
MESSAGE=" MASTER"
OUTFILE=mb_master
$(info === MASTER mode activated! ===)
endif

ifeq ("slave","$(filter slave,$(MAKECMDGOALS))")
CPPFLAGS+= -DMB_SLAVE
MESSAGE=" SLAVE"
OUTFILE=mb_slave
$(info === SLAVE mode activated! ===)
endif

ifeq ("check","$(filter check,$(MAKECMDGOALS))")
OPTFLAGS=
CPPFLAGS+= $(GLIBC_FLAGS)
CXXFLAGS+= $(SANIT_FLAGS)
#LDFLAGS+= $(GLIBC_FLAGS)
LDFLAGS+= $(SANIT_FLAGS)
MESSAGE=" with CHECK"
$(info === FULL CHECK options activated! ===)
endif

ifeq ("debug","$(filter debug,$(MAKECMDGOALS))")
OPTFLAGS=
CPPFLAGS+= $(DEBUG_FLAGS)
#LDFLAGS+= $(DEBUG_FLAGS)
DO_DEBUG=YES
MESSAGE=" with DEBUG"
MESSAGE_DEBUG="=== The size of executable file are REALLY BIG. ==="
$(info === DEDUG options activated! ===)
endif

ifeq ("fulldebug","$(filter fulldebug,$(MAKECMDGOALS))")
OPTFLAGS=
CXXFLAGS+= $(GLIBC_FLAGS)
CXXFLAGS+= $(SANIT_FLAGS)
CPPFLAGS+= $(DEBUG_FLAGS)
LDFLAGS+= $(SANIT_FLAGS)
LDFLAGS+= $(DEBUG_FLAGS)
DO_DEBUG=YES
MESSAGE=" FULL DEBUG!"
MESSAGE_DEBUG="=== The size of executable file are EXTREMELY BIG. ==="
$(info === FULL Debug options activated! ===)
endif

# === Colors (just for fun) ===
RED='\033[0;91m' \033[32m<text> 
GRE="\033[0;32m"
GRB="\033[0;92m"
YEL="\033[0;93m"
YEB="\033[1;33m"
BLU='\033[0;94m'
WHI='\033[0;97m'
NC='\033[0m' # No Color

# =====================================================================

include $(DEPFILES)

all: $(OUTFILE)
master: $(OUTFILE)
slave: $(OUTFILE)

run: clean $(OUTFILE)
check: clean $(OUTFILE)
debug: clean $(OUTFILE)
fulldebug: clean $(OUTFILE)

# ================ Linking ================================
#a.out: $(OBJLIST)
$(OUTFILE): $(OBJLIST)
	@echo -e $(GRE)"=== Linking$(MESSAGE): $@"$(NC)
#	$(CXX) $(LDFLAGS) $(OPTFLAGS) $^ -o $(OUTFILE) $(LDLIBS)
	$(LINK.o) $(OPTFLAGS) $^ $(LDLIBS) -o $@
	@echo -e $(GRE)"=== Finished$(MESSAGE) ==="$(NC)
	@ls -Fog --color $(OUTFILE)
	@echo -e $(GRE)$(MESSAGE_DEBUG)$(NC)
	sleep 2

#================== Compiling ==============================
$(OBJDIR)/%.o: %.cpp
	@echo -e $(YEL)"=== Compiling$(MESSAGE): $<"$(NC)
#	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(DEPFLAGS) $(CPPFLAGS) $(OBJDIR)/$<.d -o $@ $<
	$(COMPILE.cpp) $(INCLUDES) $(OPTFLAGS) $(DEPFLAGS)/$<.d -o $@ $<

# ================== Cleaning =============================
clean: format-linux
	@echo -e $(BLU)"=== Cleaning UP..."$(NC)
	@rm -rfv $(OBJFILES) $(DEPFILES)
	rm -rfv a.out
	@echo $(SRCDIR1)

# ================== Formatting ===========================
# Simple format current directory only
clang:
	clang-format -i --verbose *.cpp *.h

google:
	clang-format -i -style=google --verbose *.cpp *.h

# ================ ALL FILE recursively! ==================
# Reindent *.cpp to Linux code-style
format-linux:
	astyle $(ASTYLEFLAGS) -n -s2 --style=linux $(ASTYLEFILES)

format-kr:
	astyle $(ASTYLEFLAGS) -n --style=kr $(ASTYLEFILES)

format-allman:
	astyle $(ASTYLEFLAGS) -n --style=allman $(ASTYLEFILES)

format-google2:
	astyle $(ASTYLEFLAGS) -n -s2 --style=google $(ASTYLEFILES)

# Reindent *.cpp to LLVM code-style by clang-format
format-clang:
	clang-format -i -style=LLVM --verbose $(CLANGFILES)

format-google:
	clang-format -i -style=google --verbose $(CLANGFILES)

# ===
