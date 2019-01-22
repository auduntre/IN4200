#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int main (int argc, char **argv)
{
    int n = 1000000;
    int i;

    if (argc > 1) {
        n = atoi(argv[1]);
    }

    size_t array_size = n * sizeof(double);
    clock_t start, end;
    double elapsed;

    double *a = (double *) malloc (n * sizeof(double));
    double *b = (double *) malloc (n * sizeof(double));
    double *c = (double *) malloc (n * sizeof(double));

    srand(time(NULL));

    for (i = 0; i < n; i++) {
        a[i] = (double) rand();
    }

    start = clock();
    memcpy(b, a, array_size);
    end = clock();

    printf("TIME MEMCPY: %f sec\n", (double) (end - start) / CLOCKS_PER_SEC );

    start = clock();
    for (i = 0; i < n; i++) {
        c[i] = a[i];
    }
    end = clock();

    printf("TIME MANUAL COPY: %f sec\n", (double) (end - start) / CLOCKS_PER_SEC);

    printf("b[n/2] = %f, c[n/2] = %f, a[n/2] = %f\n", b[n/2], c[n/2], a[n/2]);

    free(a);
    free(b);
    free(c);
    return 0;
}
