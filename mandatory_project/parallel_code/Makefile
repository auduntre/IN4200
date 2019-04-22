CC = mpicc
CFLAGS = -O3
LDFLAGS = -L../simple-jpeg/ -lsimplejpeg -I../serial_code

CSRC = $(wildcard *.c)
OBJS = $(CSRC:.c=.o)
PROJ = parallel_main


# If compiling in parallel make sure it is in right order
.NOTPARALLEL: all
all: simple-jpeg $(PROJ)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< $(LDFLAGS)

serial_main: $(OBJS) $(DEPS)
		$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)


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
