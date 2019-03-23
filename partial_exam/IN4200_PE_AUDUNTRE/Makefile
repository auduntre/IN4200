SHELL = /bin/sh

# Compiler
CC = gcc

# Folders
#IDIR = include #small project include folder not necessary
SDIR = src
ODIR = obj

# Compiler flags
CFLAGS = -I$(SDIR) -O3
LIBS = -fopenmp

# Dependencies (headers)
DEPS = $(wildcard $(SDIR)/*.h)

# Source files
CSRC = $(wildcard $(SDIR)/*.c)

# Object files
_OBJS = $(CSRC:.c=.o)
OBJS = $(patsubst $(SDIR)/%,$(ODIR)/%,$(_OBJS))


# Make object files
$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)


# Keeping make from doing something with files named:
.PHONY: all clean


# Programs
all: PE_auduntre


# Create program executable
PE_auduntre: $(OBJS)
		$(CC) -o $@ $^ $(CFLAGS)


clean:
	rm -f PE_auduntre $(ODIR)/*.o
