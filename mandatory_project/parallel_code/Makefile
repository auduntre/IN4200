CC = mpicc
IDIR = ../serial_code

CFLAGS = -O2 -std=c99
LDFLAGS = -L../simple-jpeg -lsimplejpeg -I$(IDIR)

_DEPS = functions.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
DEPS += parallel_functions.h

CSRC = $(wildcard *.c)
OBJS = $(CSRC:.c=.o)
OBJS += $(DEPS:.h=.o)

PROJ = parallel_main


# If compiling in parallel make sure it is in right order
.NOTPARALLEL: all
all: simple-jpeg $(PROJ)

%.o: %.c $(DEPS)
		$(CC) $(CFLAGS) -c -o $@ $< $(LDFLAGS)

$(IDIR)%.o: %.c $(DEPS)
		$(CC) $(CFLAGS) -c -o $@ $< $(LDFLAGS)

parallel_main: $(OBJS)
		$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

simple-jpeg:
		$(MAKE) -C ../simple-jpeg


obj-clean:
		$(RM) *.o

exec-clean:
		$(RM) $(PROJ)

autosave-clean:
		$(RM) *~

clean:
		$(MAKE) obj-clean
		$(MAKE) exec-clean
		$(MAKE) autosave-clean
