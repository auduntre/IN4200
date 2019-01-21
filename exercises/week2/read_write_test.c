#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main (int argc, char *argv[])
{
    void write_test_files (int n);
    void read_test_files (int n);

    int n = 10000;

    if (argc > 1) {
        n = atoi(argv[1]);
    }

    printf("n = %d\n", n);

    srand(time(NULL));

    write_test_files(n);
    read_test_files(n);

    return 0;
}


void write_test_files (int n)
{
    void print_elapsed (char *what_is_timed, clock_t start, clock_t end);
    
    FILE *outfile_binary;
    FILE *outfile_ASCII;

    int *random_values = (int *) malloc (n * sizeof(int));

    for (int i = 0; i < n; i++) {
        random_values[i] = rand();
    }

    clock_t start, end;

    outfile_binary = fopen("test.bin", "wb");
    outfile_ASCII = fopen("test.txt", "w");

    start = clock();
    fwrite(random_values, sizeof(int), n, outfile_binary);
    end = clock();
    print_elapsed("write binary", start, end);

    start = clock();
    for (int i = 0; i < n; i++) {
        fprintf(outfile_ASCII, "%u ", random_values[i]);
    }
    end = clock();
    print_elapsed("write ASCII", start, end);
   
    free(random_values); 
    fclose(outfile_binary);
    fclose(outfile_ASCII);
}


void read_test_files (int n)
{
    void print_elapsed (char *what_is_timed, clock_t start, clock_t end);

    FILE *infile_binary;
    FILE *infile_ASCII;

    int *invalues_binary = (int *) malloc (n * sizeof(int));
    int *invalues_ASCII = (int *) malloc (n * sizeof(int));

    clock_t start, end;
    
    infile_binary = fopen("test.bin", "rb");
    infile_ASCII = fopen("test.txt", "r");

    start = clock();
    fread(invalues_binary, sizeof(int), n, infile_binary);
    end = clock();
    print_elapsed("read binary", start, end);

    start = clock();
    for (int i = 0; i < n; i++) {
        fscanf(infile_ASCII, "%d ", &invalues_ASCII[i]);
    }
    end = clock();
    print_elapsed("read ASCII", start, end);

    for (int i = 0; i < n; i++) {
        if (invalues_ASCII[i] != invalues_binary[i]) {
            printf("ERROR IN READING OR WRITING AS ASCII AND BINARY ");
            printf("DOESNT GIVE THE SAME VALUES.\n");
            break;
        }
    }

    free(invalues_binary);
    free(invalues_ASCII);

    fclose(infile_binary);
    fclose(infile_ASCII);
}


void print_elapsed (char *what_is_timed, clock_t start, clock_t end)
{
    double elapsed = (double) (end - start) / CLOCKS_PER_SEC;
    printf("Time to %s: %f sec. \n", what_is_timed, elapsed);
}
