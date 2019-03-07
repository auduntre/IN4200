CC = gcc
CFLAGS = -O3

MOBJ = memband.o


%.o: %.c
	$(CC) $^ -c -o $@ $(CFLAGS)


memband: $(MOBJ)
	$(CC) $^ -o $@ $(CFLAGS)


.PHONY: clean

clean:
	rm -f *.o
