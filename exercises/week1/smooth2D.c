#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>


int main (int argc, char **argv)
{
    void smooth (double **v_new, double **v, int n, int m, double c);
    
    int i, j;
    int n = 10;
    int m = 10;

    if (argc > 1) {
        n = atoi(argv[1]);

        if (argv > 2) {
            m = atoi(argv[2]);
        }
    }

    srand(time);

    double **v = (double **) malloc (n * sizeof(double *));
    double **v_new = (double **) malloc (n * sizeof(double *));

    for (i = 0; i < n; i++) {
        v[i] = (double *) malloc (m * sizeof(double));
        v_new[i] = (double *) malloc (m * sizeof(double));
    }


    for (i = 0; i < n; i++) {
        free(v[i]);
        free(v_new[i]);
    } 

    free(v);
    free(v_new);

    return 0;
}


void smooth (double **v_new, double **v, int n, int m, double c)
{
    int i, j;

    double k = 1 - 4*c;

    for (i = 1; i < n-1; i++) {
        // Boundaries
        v_new[i][0] = v[i][0];
        
        // Internal smoothing
        for (j = 1; j < m-1; j++) {
            v_new[i][j] = k * v[i][j] + c * (v[i-1, j] + v[i, j-1] + 
                          v[i+1][j] + v[i][j+1]);
        }
        
        // Boundaries
        v_new[i][m-1] = v[i][m-1];
    }

    // Boundaries
    for (j = 0; j < m; j++) {
        v_new[0][j] = v[0][j];
        v_new[n-1][j] = v[n-1][j];
    }
}
