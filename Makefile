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
MAKEFLAGS+= -j4

# === COMMON: Version/standard & Directories & files & libraries ===
CXX_VER=c++20
EXEC_FILE=a.out
SRCDIRS= .
SUBDIRS= include sources

INCLUDES = -I.
LIBS=libmodbus libconfig++
# open62541
LDLIBS= -lrt -lpthread -lmbedtls -lmbedx509 -lmbedcrypto -lopen62541
#OPEN62541_O= include/open62541/open62541.o
OBJDIR =./tmp/obj
CC=$(CXX)

# === C/CPP main flags configuration ===
#CPPFLAGS= -std=$(CXX_VER)
CXXFLAGS= -std=$(CXX_VER) -Wall -Wextra -Wpedantic -Wfatal-errors
#LDFLAGS = -Wall #-std=$(CXX_VER)
DEPFLAGS= -MD -MF $(OBJDIR)
OPTFLAGS= -flto=auto -O2 
#-Os -s -Wl,--as-needed

include .mk/folders.mk

include .mk/flags.mk

include .mk/checks.mk

include .mk/colors.mk

include .mk/main.mk

include .mk/formatting.mk


# === eof ===

