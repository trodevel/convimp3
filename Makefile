# Makefile for libconvimp3
# Copyright (C) 2015 Sergey Kolevatov

###################################################################

VER := 0

MODE ?= debug

PROJECT := convimp3


###################################################################

EXT_LIBS=-lmp3lame

###################################################################

LIBNAME=libconvimp3

###################################################################

ifeq "$(MODE)" "debug"
    OBJDIR=./DBG
    BINDIR=./DBG

    CFLAGS := -Wall -std=c++0x -ggdb -g3
    LFLAGS := -Wall -lstdc++ -lrt -ldl -lm -g
    LFLAGS_TEST := -Wall -lstdc++ -lrt -ldl -g -L.  -lm

    TARGET=example
else
    OBJDIR=./OPT
    BINDIR=./OPT

    CFLAGS := -Wall -std=c++0x
    LFLAGS := -Wall -lstdc++ -lrt -ldl -lm
    LFLAGS_TEST := -Wall -lstdc++ -lrt -ldl -L. -lm

    TARGET=example
endif

###################################################################

CC=gcc

LDSHARED=gcc
CPP=gcc -E
INCL = -I.


STATICLIB=$(LIBNAME).a
SHAREDLIB=
SHAREDLIBV=
SHAREDLIBM=
LIBS=$(STATICLIB) $(SHAREDLIBV)

AR=ar rc
RANLIB=ranlib
LDCONFIG=ldconfig
LDSHAREDLIBC=-lc
TAR=tar
SHELL=/bin/sh
EXE=

#vpath %.cpp .

SRCC = codec.cpp
OBJS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCC))

LIB_NAMES = lameplus wave
LIBS = $(patsubst %,$(BINDIR)/lib%.a,$(LIB_NAMES))

all: static

static: $(TARGET)

check: test

test: all teststatic

teststatic: static
	@echo static test is not ready yet, dc10

$(BINDIR)/$(STATICLIB): $(OBJS)
	$(AR) $@ $(OBJS)
	-@ ($(RANLIB) $@ || true) >/dev/null 2>&1

$(OBJDIR)/%.o: %.cpp
	@echo compiling $<
	$(CC) $(CFLAGS) -DPIC -c -o $@ $< $(INCL)

$(TARGET): $(BINDIR) $(BINDIR)/$(TARGET)
	ln -sf $(BINDIR)/$(TARGET) $(TARGET)
	@echo "$@ uptodate - ${MODE}"

$(BINDIR)/$(TARGET): $(LIBS) $(OBJDIR)/$(TARGET).o $(OBJS) $(BINDIR)/$(STATICLIB)
	$(CC) $(CFLAGS) -o $@ $(OBJDIR)/$(TARGET).o $(BINDIR)/$(LIBNAME).a $(LIBS) $(EXT_LIBS) $(LFLAGS_TEST)

$(BINDIR)/liblameplus.a:
	cd ../lameplus; make; cd $(project)
	ln -sf ../../lameplus/$@ $(BINDIR)

$(BINDIR)/libwave.a:
	cd ../wave; make; cd $(project)
	ln -sf ../../wave/$@ $(BINDIR)

$(BINDIR):
	@ mkdir -p $(OBJDIR)
	@ mkdir -p $(BINDIR)

clean:
	rm $(OBJDIR)/*.o $(TARGET) $(BINDIR)/$(TARGET) $(BINDIR)/$(STATICLIB)
