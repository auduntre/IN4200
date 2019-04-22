CC = gcc
CFLAGS = -O3
LDFLAGS = -L../simple-jpeg/ -lsimplejpeg

DEPS = $(wildcard *.h)
CSRC = $(wildcard *.c)
OBJS = $(CSRC:.c=.o)
PROJ = serial_main

.NOTPARALLEL: all
all: simple-jpeg $(PROJ)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< $(LDFLAGS)
		

serial_main: $(OBJS) $(DEPS)
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
		#$(MAKE) -C ../simple-jpeg clean


