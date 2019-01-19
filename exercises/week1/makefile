CC = gcc
CFLAGS= -lm -O3


limit: limit.c
	$(CC) -o $@ $^ $(CFLAGS)


array_allocation: array_allocation_1D.c
	$(CC) -o $@ $^ $(CFLAGS)


matrix_assigning: matrix_assigning.c
	$(CC) -o $@ $^ $(CFLAGS)


3Dinto1D: 3Dinto1D.c
	$(CC) -o $@ $^ $(CFLAGS)


temp_measure: temp_measure.c 
	$(CC) -o $@ $^ $(CFLAGS)


smooth2D: smooth2D.c
	$(CC) -o $@ $^ $(CFLAGS) -fopenmp


quicksort: quicksort.c
	$(CC) -o $@ $^ $(CFLAGS)


clean:
	rm limit array_allocation matrix_assigning 3Dinto1D temp_measure
