#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>


static const int PRINT_MAX = 10;


int main (int argc, char **argv)
{
    void smooth (double **v_new, double **v, int n, int m, double c);
    void print_matrix (double **x, int n, int m);
    
    int i, j, k;
    int n = 10;
    int m = 10;
    int n_smooth = 10;

    const double noise_dnm = (double) RAND_MAX * 10000;
    double dnm = (double) (n + m);
    double start, end;

    if (argc > 1) {
        n = atoi(argv[1]);

        if (argc > 2) {
            m = atoi(argv[2]);

            if (argc > 3) {
                n_smooth = atoi(argv[3]);
            }
        }
    }

    double **v = (double **) malloc (n * sizeof(double *));
    double **v_new = (double **) malloc (n * sizeof(double *));

    for (i = 0; i < n; i++) {
        v[i] = (double *) malloc (m * sizeof(double));
        v_new[i] = (double *) malloc (m * sizeof(double));
    }


    // Assign random values
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            v[i][j] = (double) (i + j) / dnm + (double) rand() / noise_dnm;
        }
    }

    print_matrix(v, n, m);

    start = omp_get_wtime();
    for (k = 0; k < n_smooth; k++) {
        smooth(v_new, v, n, m, 0.5);
    }
    end = omp_get_wtime();

    print_matrix(v_new, n, m);

    printf("Elapsed time: %f sec\n", end - start);

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
            v_new[i][j] = k * v[i][j] + c * (v[i-1][j] + v[i][j-1] + 
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


void print_matrix (double **x, int n, int m)
{
    int i, j;

    if (n <= PRINT_MAX && m <= PRINT_MAX) {    
        printf("-----------------------------\n");
        for (i = 0; i < n; i++) {
            for (j = 0; j < m; j++) {
                printf("%f, ", x[i][j]);
            }
            printf("\n");
        }
    }
}
