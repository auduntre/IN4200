SHELL = /bin/sh

# Keeping make from doing something with files named:
.PHONY: all clean

# Explicitly set file suffixes, not necessary
.SUFFIXES:
.SUFFIXES: .c .h. .o

# Compiler
CC = gcc

# Folders
IDIR = src #small project include folder not necessary
SDIR = src
ODIR = obj

# Compiler flags
CFLAGS = -I$(IDIR) -O3
LIBS = -fopenmp

# Dependencies (headers)
_DEPS = PE_functions_auduntre.h
DEPS = $(patsubst %,$(IDIR)/%,%(_DEPS))

# Object files
_OBJS = PE_functions_auduntre.o PE_main_auduntre.c
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))


# Programs
all: PE_auduntre


# Make object files
$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)


# Create program executable
PE_auduntre: $(OBJ)
		$(CC) -o $(@) $^ $(CFLAGS)


clean:
	rm -f $(ODIR)/*.o $(all)
