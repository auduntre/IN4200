CC = gcc
CFLAGS = -lm -O3


read_write_test: read_write_test.c
	$(CC) -o $@ $^ $(CFLAGS)


array_copy: array_copy.c
	$(CC) -o $@ $^ $(CFLAGS)


smooth3D: smooth3D.c
	$(CC) -o $@ $^ $(CFLAGS)


.PHONY: clean
clean:
	rm read_write_test array_copy smooth3D
