#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main (int argc, char **argv)
{
    int n = 1000;
    int m = 1000;

    int i, j;

    clock_t start, end;
    double elapsedc, elapsedr;

    double **A = (double **) malloc (n * sizeof(double *));
    double **B = (double **) malloc (n * sizeof(double *));


    for (i = 0; i < n; i++) {
        A[i] = (double *) malloc (m * sizeof(double));
        B[i] = (double *) malloc (m * sizeof(double));        
    } 

    start = clock();
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            A[i][j] = 1.0;
        }
    }
    end = clock();
    elapsedr = (double) (end - start) / CLOCKS_PER_SEC;
    printf("Row major assignment took: %f seconds \n", elapsedr);
    
    start = clock();
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            B[j][i] = 1.0;
        }
    }
    end = clock();
    elapsedc = (double) (end - start) / CLOCKS_PER_SEC;
    printf("Column major assignment took: %f seconds \n", elapsedc);

    printf("Row vs Column major speedup: %f \n", elapsedc / elapsedr);

    for (i = 0; i < n; i++) {
        free(A[i]);
        free(B[i]);
    }

    free(A);
    free(B);

    return 0;
}
