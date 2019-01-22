CC = gcc
CFLAGS = -O3 -lm


read_write_test: read_write_test.c
	$(CC) -o $@ $^ $(CFLAGS)


array_copy: array_copy.c
	$(CC) -o $@ $^ $(CFLAGS)


smooth3D: smooth3D.c
	$(CC) -o $@ $^ $(CFLAGS)


clean:
	rm read_write_test array_copy smooth3D
