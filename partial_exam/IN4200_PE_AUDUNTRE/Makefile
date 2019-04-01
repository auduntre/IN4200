SHELL = /bin/sh

# Compiler
CC = gcc

# Folders
#IDIR = include #small project include folder not necessary
SDIR = src
ODIR = obj

# Compiler flags
CFLAGS = -I$(SDIR) -O3
DFLAGS = 
LIBS = -fopenmp -lm

# Dependencies (headers)
DEPS = $(wildcard $(SDIR)/*.h)

# Source files
CSRC = $(wildcard $(SDIR)/*.c)

# Object files
_OBJS = $(CSRC:.c=.o)
OBJS = $(patsubst $(SDIR)/%,$(ODIR)/%,$(_OBJS))

# Make object files
$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS) $(DFLAGS) $(LIBS)


# Keeping make from doing something with files named:
.PHONY: all clean


# Programs
all: PE_15301


# Create program executable
PE_15301: $(OBJS)
		$(CC) -o $@ $^ $(CFLAGS) $(LIBS)


clean:
	rm -f PE_15301 $(ODIR)/*.o
