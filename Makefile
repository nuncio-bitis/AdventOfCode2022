#-------------------------------------------------------------------------------
# Makefile for AdventOfCode2022 directory
#
# J. Parziale  December 2022
#-------------------------------------------------------------------------------

MV=mv -v
FIND=/usr/bin/find
RM=rm -vf

OSTYPE := $(shell uname)

MAKEFILE_DIR_=$(shell pwd)
MAKEFILE_DIR=$(abspath $(MAKEFILE_DIR_))

#-------------------------------------------------------------------------------

# GCC versions under 6.0 can only use c++ std "c++0x".
# Newer ones can use c++17, c++2a, c++2b, or c++20
# C++20 - avail since GCC 8, still experimental. Use c++2a in GCC 9 and earlier.
# C++17 - avail since GCC 5, default in GCC 11
# C++14 - avail since GCC 6.1, default in GCC 10
# C++11 - avail since GCC 4.8.1, using C++0x or C++11 as follows:
#         GCC 4.3 thru 4.6 use c++0x
#         GCC 4.7 thru 4.8 use c++11
GCC_VER=$(shell $(CXX) -dumpversion | cut -f1-2 -d. | sed 's/\.//g')
MIN_GCC_VER = 60

# gcc 10.x just shows up as 10 instead of 10x
ifeq ($(GCC_VER),10)
  GCC_VER=100
endif

GCCVERSIONGT60 := $(shell expr $(GCC_VER) \>= $(MIN_GCC_VER))
OSTYPE := $(shell uname)
ifeq "$(GCCVERSIONGT60)" "1"
    GCC_STD=-std=c++20
else
    GCC_STD=-std=c++0x
endif

ifneq "$(OSTYPE)" "Darwin"
	LIBS += -lrt
else
	GCC_STD=-std=c++20
	INCLUDES = -I../mqueue-mac -I /Library/Developer/CommandLineTools/SDKs/MacOSX12.1.sdk/usr/include/sys/_types
	OBJS = ../mqueue-mac/mqueue.o
endif

#-------------------------------------------------------------------------------

COMPILER = /usr/bin/g++
CC       = /usr/bin/gcc
CXX      = /usr/bin/g++
GCC      = /usr/bin/gcc
GXX      = /usr/bin/g++
LINK     = /usr/bin/g++

LIB_FILE=

INCLUDES += -I /usr/include
LIBS += -lpthread -lxml2

CXXFLAGS = $(GCC_STD) -O2 -g -Wall -fmessage-length=0 $(INCLUDES) $(LIBDIRS) $(LIBS)
CFLAGS = -O2 -g -Wall  -fmessage-length=0 $(INCLUDES) $(LIBDIRS) $(LIBS)

LDFLAGS = -pthread

OBJ_TARGETS =

CXX_TARGETS = day11

#-------------------------------------------------------------------------------

.cpp:.o
	$(CXX) $(CXXFLAGS) $< -o $@ $(LIBS)

.c:.o
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

#all: begin ctargs otargs end
all: begin ctargs end

begin:
	@echo "--------------------------------------------------------------------------------"
	@echo " Building targets in $(MAKEFILE_DIR)"
	@echo "--------------------------------------------------------------------------------"
	@echo

ctargs: $(C_TARGETS) $(CXX_TARGETS)
	@echo "--------------------------------------------------------------------------------"
	@echo " C/C++ Targets done"
	@echo "--------------------------------------------------------------------------------"
	@echo

otargs: $(OBJ_TARGETS)
	@echo "--------------------------------------------------------------------------------"
	@echo " OBJ Targets done"
	@echo "--------------------------------------------------------------------------------"
	@echo

end:
	@echo "--------------------------------------------------------------------------------"
	@echo " FINISHED building targets in $(MAKEFILE_DIR)"
	@echo "--------------------------------------------------------------------------------"
	@echo

#-------------------------------------------------------------------------------

tidy:
	$(RM) -vf *.d *.map *.log
	$(RM) -rf *.dSYM

clean: tidy
	$(RM) -vf *.o

clobber: clean
	$(RM) -vf $(C_TARGETS) $(CXX_TARGETS)
	$(RM) -vf $(LIB_FILE)

echo:
	@echo
	@echo "OSTYPE      = $(OSTYPE)"
	@echo "GCC_VER     = $(GCC_VER)"
	@echo "MIN_GCC_VER = $(MIN_GCC_VER)"
	@echo
	@echo "MAKEFILE_DIR = $(MAKEFILE_DIR)"
	@echo "INCLUDES  = $(INCLUDES)"
	@echo "LIBDIRS   = $(LIBDIRS)"
	@echo "LIBS      = $(LIBS)"
	@echo "LDFLAGS   = $(LDFLAGS)"
	@echo "CFLAGS    = $(CFLAGS)"
	@echo "CXXFLAGS  = $(CXXFLAGS)"
	@echo "OBJS      = $(OBJS)"
	@echo "TARGETS   = $(TARGETS)"
	@echo
	@echo "XXX = ${XXX}"  # Specify "make XXX=yourString" on the command line
	@echo

#-------------------------------------------------------------------------------
# Don't need to specify unless there are special rules.

#t-trunc : t-trunc.c
#	$(CC) $(CFLAGS) $< -o t-trunc -lm

#-------------------------------------------------------------------------------
