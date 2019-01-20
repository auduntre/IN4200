#include <stdio.h>
#include <stdlib.h>
#include <time.h>


static const int size_int = sizeof(int);


int main (int argc, char *argv[])
{
    void write_test_files (int n);

    int n = 10000;

    if (argc > 1) {
        n = atoi(argv[1]);
    }

    printf("n = %d\n", n);

    srand(time(NULL));

    write_test_files(n);

    return 0;
}


void write_test_files (int n)
{
    void print_elapsed (char *what_is_timed, clock_t start, clock_t end);
    
    FILE *outfile_binary;
    FILE *outfile_ASCII;

    int *random_values = (int *) malloc (n * size_int);

    for (int i = 0; i < n; i++) {
        random_values[i] = rand();
    }

    clock_t start, end;

    outfile_binary = fopen("test.bin", "wb");
    outfile_ASCII = fopen("test.txt", "w");

    start = clock();
    fwrite(random_values, size_int, n, outfile_binary);
    end = clock();
    print_elapsed("write binary", start, end);

    start = clock();
    fwrite(random_values, size_int, n, outfile_ASCII);
    end = clock();
    print_elapsed("write ASCII", start, end);
    
    fclose(outfile_binary);
    fclose(outfile_ASCII);
}


void print_elapsed (char *what_is_timed, clock_t start, clock_t end)
{
    double elapsed = (double) (end - start) / CLOCKS_PER_SEC;
    printf("Time to %s: %f sec. \n", what_is_timed, elapsed);
}
