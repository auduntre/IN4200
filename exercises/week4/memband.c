/* Program for measuring the memory bandwidth of the cpu */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>


int main (int argc, char **argv)
{
    int N = 10000;
    double s = 0.3;

    if (argc > 1) {
        N = atoi(argv[1]);
        
        if (argc > 2) {
            s = atof(argv[2]);;    
        }
    }

    double *A = (double *) malloc (N * sizeof(double));
    double *B = (double *) malloc (N * sizeof(double));

    srand(time(NULL));

    /* Initialize array */
    for (int j = 0; j < N; j++) {
        B[j] = (double) (rand() % 100);
    }

    clock_t start;
    clock_t end;
   
    start = clock();
    for (int i = 0; i < N; i++) {
        A[i] = s*B[i];
    }
    end = clock();

    double cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("CPU time used was: %f sec.\n", cpu_time);

    return 0;
}

