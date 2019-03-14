SHELL = /bin/sh

# Keeping make from doing something with files named
.PHONY: clean

# Explicitly set file suffixes, not nesseacry
.SUFFIXES:
.SUFFIXES: .c .h. .o

# Compiler
CC = gcc

# Folders
IDIR = include
ODIR = obj

# Compiler flags
CFLAGS = -I$(IDIR)
LIBS = -fopenmp

# Dependencies (headers)
_DEPS = PE_functions_auduntre.h
DEPS = $(patsubst %,$(IDIR)/%,%(_DEPS))

# Object files

_OBJS = PE_functions_auduntre.o PE_main_auduntre.c
OBJ = $(patsubst %,$())
