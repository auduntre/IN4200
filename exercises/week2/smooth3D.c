#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define NNVALUES 4


int main (int argc, char **argv)
{
    void free3D (double ***A, int nx, int ny);
    double *** double_malloc3D (int nx, int ny, int nz);
    void print_array3D (char *Arrname, double ***Arr, int nx, int ny, int nz);
    
    double *** smoothing_iterator3D (double ***u, double ***v, int nx, int ny, 
                               int nz, int iterations);

    unsigned int ns[NNVALUES];
    
    for (int xi = 0; xi < NNVALUES; xi++) {
        if (xi+1 < argc) {
            ns[xi] = atoi(argv[xi+1]);
        }
        else {
            ns[xi] = 10;
        }
    }

    unsigned int nx = ns[0];
    unsigned int ny = ns[1];
    unsigned int nz = ns[2];
    unsigned int iterations = ns[3];
  
    // Allocation of memory
    double ***v = double_malloc3D(nx, ny, nz);
    double ***u = double_malloc3D(nx, ny, nz);
    
    // Assignmnet of values
    double dnm = (nx - 1.0) * (ny - 1.0) * (nz - 1.0);
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            for (int k = 0; k < nz; k++) {
                v[i][j][k] = 2.0 + sin(i * j * k * M_PI / dnm);
                u[i][j][k] = 0.0;
            }
        }
    }

    v = smoothing_iterator3D(u, v, nx, ny, nz, iterations);

    if (nx * ny * nz <= 1000) {
        print_array3D("u", u, nx, ny, nz);
        print_array3D("v", v, nx, ny, nz);
    }

    free3D(u, nx, ny);
    free3D(v, nx, ny);
    return 0;
}


void free3D (double ***A, int nx, int ny)
{
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            free(A[i][j]);
        }
        free(A[i]);
    }
    free(A);
}


double *** double_malloc3D (int nx, int ny, int nz)
{
    double ***A = (double ***) malloc (nx * sizeof(double **));
    
    for (int i = 0; i < nx; i++) {
        A[i] = (double **) malloc (ny * sizeof(double *));

        for (int j = 0; j < ny; j++) {
            A[i][j] = (double *) malloc (nz * sizeof(double));
        }
    }

    return A;
}


double *** smooth3D (double ***u, double ***v, int nx, int ny, int nz)
{
    /* u[i][j][k] = 
     *              v[i][j][k] + (v[i-1][j][k] + v[i][j-1][k] + v[i][j][k-1]
     *                            - 6 * v[i][j][k] + v[i+1][j][k] 
     *                            + v[i][j+1][k] + v[i][j][k+1]) / 6
     *            = 
     *              (v[i-1][j][k] + v[i][j-1][k] + v[i][j][k-1] 
     *               + v[i+1][j][k] + v[i][j+1][k] + v[i][j][k+1]) / 6 
     */
    
    int i, j, k;
    
    // smoothing of v into u
    for (i = 1; i < (nx - 1); i++) {
        for (j = 1; j < (ny - 1); j++) {
            for (k = 1; k < (nz - 1); k++) {
                u[i][j][k] = v[i][j][k] + (v[i-1][j][k] + v[i][j-1][k]
                                           + v[i][j][k+1] - 6 * v[i][j][k]
                                           + v[i-1][j][k] + v[i][j+1][k]
                                           + v[i][j][k+1]) / 6.0;
            }
        }
    }

    return u;
}


double *** smoothing_iterator3D (double ***u, double ***v, int nx, int ny,
                                 int nz, int iterations)
{
    double *** smooth3D (double ***u, double ***v, int nx, int ny, int nz);
    void copy_array3D (double ***u, double ***v, int nx, int ny, int nz);

    unsigned int iter = 0;

    copy_array3D(v, u, nx, ny, nz);   
    
    while (iter < iterations) {
        u = smooth3D(u, v, nx, ny, nz);
        copy_array3D(u, v, nx, ny, nz);   
        // set v = u for next iteration 
        iter++;
    }

    return v;
}


void print_array3D (char *Arrname, double ***Arr, int nx, int ny, int nz)
{
    printf("-----------------------\n");
    printf("%s = \n", Arrname);

    for (int i = 0; i < nx; i++) {
        printf("i = %d: \n", i);
        for (int j = 0; j < ny; j++) {
            for (int k = 0; k < nz; k++) {
                printf("%f, ", Arr[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
}


void copy_array3D (double ***u, double ***v, int nx, int ny, int nz)
{
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            for (int k = 0; k < nz; k++) {
                v[i][j][k] = u[i][j][k];
            }
        }
    }
}
