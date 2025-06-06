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
MAKEFLAGS+= -j8

# === COMMON: Version/standard & Directories & files & libraries ===
CXX_VER=c++20
EXEC_FILE=a.out
SRCDIRS= .
SUBDIRS= include sources

INCLUDES = -I.
LIBS=libmodbus libconfig++ open62541
LDLIBS= -lrt -lpthread -lmbedtls 
#-lmbedx509 -lmbedcrypto #OPEN62541_O= include/open62541/open62541.o
OBJDIR =./tmp/obj
CC=$(CXX)

# === C/CPP flags configuration ===
#CPPFLAGS= -std=$(CXX_VER)
CXXFLAGS= -std=$(CXX_VER) -Wall -Wextra -Wpedantic -Wfatal-errors
#LDFLAGS = -Wall #-std=$(CXX_VER)
DEPFLAGS= -MD -MF $(OBJDIR)
OPTFLAGS= -flto=auto -O2 #-Os -s -Wl,--as-needed


# ==== Folders processing ===
LDLIBS+=$(foreach lib,$(LIBS),$(shell pkg-config --libs --cflags $(lib)))

ALLDIRS= $(foreach dir,$(SUBDIRS),$(shell find -L $(dir) -maxdepth 1 -type d))
SRCDIRS+= $(ALLDIRS)

$(shell mkdir $(OBJDIR) 2>/dev/null)
$(foreach dir,$(SRCDIRS),$(shell mkdir $(OBJDIR)/$(dir) 2>/dev/null))

SRCFILES=$(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.cpp))
DEPFILES=$(foreach dir,$(SRCDIRS),$(wildcard $(OBJDIR)/$(dir)/*.cpp.d))

OBJLIST =$(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCFILES)) 
#OBJFILES=$(foreach dir,$(SRCDIRS),$(wildcard $(OBJDIR)/$(dir)/*.o))

#OUTF=$(shell ls -Fog $(EXEC_FILE))


# === Add more warning. "Stolen" here https://codeforces.com/blog/entry/15547
WARN1_FLAGS= -Wshadow -Wformat=2 -Wfloat-equal -Wconversion
WARN2_FLAGS= -Wcast-align -Wcast-qual
WARN3_FLAGS= -Wduplicated-cond -Wlogical-op -Wshift-overflow=2
CXXFLAGS+=$(WARN1_FLAGS) $(WARN2_FLAGS) $(WARN3_FLAGS)

# === For debugging & "deep" research
GLIBC_FLAGS= -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 
SANIT_FLAGS= -fstack-protector -fsanitize=address -fsanitize=undefined -fno-sanitize-recover
DEBUG_FLAGS= -g -DDEBUG_FLAG
#CXXFLAGS+= $(GLIBC_FLAGS)
#CXXFLAGS+= $(SANIT_FLAGS)
#CXXFLAGS+= -fanalyzer

# === Check for DEBUG build ===
MESSAGE_DEBUG="===+++===+++==="

ifeq ("master","$(filter master,$(MAKECMDGOALS))")
CPPFLAGS+= -DMB_MASTER
MESSAGE=" MASTER"
EXEC_FILE=mb_master
$(info === MASTER mode activated! ===)
endif

ifeq ("slave","$(filter slave,$(MAKECMDGOALS))")
CPPFLAGS+= -DMB_SLAVE
MESSAGE=" SLAVE"
EXEC_FILE=mb_slave
$(info === SLAVE mode activated! ===)
endif

# === Debug & other tricks ===

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
ifneq ("/usr/bin/dash","$(shell readlink -f /bin/sh)")
E=-e
endif
RED='\033[0;91m'
ECHO_RED=@echo $(E) '\033[0;91m'
GRE='\033[0;32m'
ECHO_GRE=@echo $(E) '\033[0;32m'
YEL='\033[0;93m'
ECHO_YEL=@echo $(E) '\033[0;93m'
BLU='\033[0;94m'
ECHO_BLU=@echo $(E) '\033[0;94m'
WHI='\033[0;97m'
NC='\033[0m'     # No Color

# =====================================================================

include $(DEPFILES)

all: $(EXEC_FILE)
master: $(EXEC_FILE)
slave: $(EXEC_FILE)

run: clean $(EXEC_FILE)
check: clean $(EXEC_FILE)
debug: clean $(EXEC_FILE)
fulldebug: clean $(EXEC_FILE)

# ================ Linking ================================
$(EXEC_FILE): $(OBJLIST)
	$(ECHO_GRE)"=== Linking$(MESSAGE): $@"$(NC)
	$(LINK.o) $(OPTFLAGS) $^ $(LDLIBS) -o $@
	$(ECHO_GRE)"=== Finished$(MESSAGE) ==="$(NC)
	@ls -Fog --color $(EXEC_FILE)
	$(ECHO_GRE)$(MESSAGE_DEBUG)$(NC)
	sleep 2
#	$(CXX) $(LDFLAGS) $(OPTFLAGS) $^ -o $(EXEC_FILE) $(LDLIBS)
#	$(LINK.o) $(OPTFLAGS) $(OPEN62541_O) $^ $(LDLIBS) -o $@

#================== Compiling ==============================
$(OBJDIR)/%.o: %.cpp
	$(ECHO_YEL)"=== Compiling$(MESSAGE): $<"$(NC)
	$(COMPILE.cpp) $(INCLUDES) $(OPTFLAGS) $(DEPFLAGS)/$<.d -o $@ $<
#	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(DEPFLAGS) $(CPPFLAGS) $(OBJDIR)/$<.d -o $@ $<

# ================== Cleaning =============================
clean: format
	$(ECHO_BLU)"=== Cleaning UP with... $<"$(NC)
	@rm -rfv $(EXEC_FILE)
#	find test -maxdepth 5 -type f -name *.o -print -delete
	find . -type f \( -name "*.d" -or -name "*.o" -or -name "a.out" \) -print -delete

# ================== Formatting ===========================
# Simple format current directory only
format: AStyle-linux

kr: AStyle-KR

clang: Clang-LLVM

google: Clang-google

ASTYLEFLAGS= -k1 -W3 -xg -xb -xj -xp -c -O -H
ASTYLEFILES=$(foreach dir,$(SRCDIRS),$(dir)/*.cpp,*.h)
CLANGFILES =$(foreach dir,$(SRCDIRS),$(dir)/*.cpp $(dir)/*.h)

# ================ Styling ALL SRC FILES recursively! ==================
AStyle-linux:
	@echo -e $(BLU)"=== Formatting with: $@"$(NC)
	astyle $(ASTYLEFLAGS) -n -s2 --style=linux $(ASTYLEFILES)

AStyle-KR:
	@echo -e $(BLU)"=== Formatting with: $@"$(NC)
	astyle $(ASTYLEFLAGS) -n --style=kr $(ASTYLEFILES)

AStyle-allman:
	@echo -e $(BLU)"=== Formatting with: $@"$(NC)
	astyle $(ASTYLEFLAGS) -n --style=allman $(ASTYLEFILES)

AStyle-google:
	@echo -e $(BLU)"=== Formatting with: $@"$(NC)
	astyle $(ASTYLEFLAGS) -n -s2 --style=google $(ASTYLEFILES)

Clang-LLVM:
	@echo -e $(BLU)"=== Formatting with: $@"$(NC)
	clang-format -i -style=LLVM --verbose $(CLANGFILES)

Clang-google:
	@echo -e $(BLU)"=== Formatting with: $@"$(NC)
	clang-format -i -style=google --verbose $(CLANGFILES)

# === eof ===

