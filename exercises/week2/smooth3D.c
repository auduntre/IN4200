#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define NNVALUES 4


int main (int argc, char **argv)
{
    void free3D (double ***A, int nx, int ny);
    double *** double_malloc3D (double *A_storage, int nx, int ny, int nz);
    void print_array3D (char *Arrname, double ***Arr, int nx, int ny, int nz);
    
    void smoothing_iterator3D (double ***u, double ***v, int nx, int ny, 
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
 
    clock_t start, end;
    double FLOP, elapased;

    // Allocation of memory
    double *v_storage;
    double *u_storage;

    double ***v = double_malloc3D(v_storage, nx, ny, nz);
    double ***u = double_malloc3D(u_storage, nx, ny, nz);
    
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

    start = clock();
    smoothing_iterator3D(u, v, nx, ny, nz, iterations);
    end = clock();


    if (nx * ny * nz <= 1000) {
        print_array3D("u", u, nx, ny, nz);
        print_array3D("v", v, nx, ny, nz);
    }

    elapased = (double) (end - start) / CLOCKS_PER_SEC;
    FLOP = 9 * nx * ny * nz * iterations; 
    printf("GFLOPS = %.4f\n", 1.E-9 * FLOP / elapased);

    free3D(u, nx, ny);
    free3D(v, nx, ny);
    printf("Freeing underlaying storage...");
    //free(v_storage);
    //free(u_storage);
    printf("DONE.\n");
    return 0;
}


void free3D (double ***A, int nx, int ny)
{
    printf("Freeing 3D Array ...");
    
    for (int i = 0; i < nx; i++) {
        free(A[i]);
    }
    free(A);
    printf("DONE.\n");
}


double *** double_malloc3D (double *A_storage, int nx, int ny, int nz)
{
    printf("Allocating memory to 3D array ...");
    A_storage = (double *) malloc (nx * ny * nz * sizeof(double));
    double ***A = (double ***) malloc (nx * sizeof(double **));
    
    for (int i = 0; i < nx; i++) {
        A[i] = (double **) malloc (ny * sizeof(double *));

        for (int j = 0; j < ny; j++) {
            A[i][j] = &(A_storage[i*ny*nz + j*nz]);
                // = (double *) malloc (nz * sizeof(double));
        }
    }

    printf("DONE\n");
    return A;
}


void smooth3D (double ***u, double ***v, int nx, int ny, int nz)
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
    const double c = 1.0 / 6.0;
    const double q = 6.0;

    // smoothing of v into u
    for (i = 1; i < (nx - 1); i++) {
        for (j = 1; j < (ny - 1); j++) {
            for (k = 1; k < (nz - 1); k++) {
                u[i][j][k] = v[i][j][k] + c * (v[i-1][j][k] + v[i][j-1][k]
                                            + v[i][j][k+1] - q * v[i][j][k]
                                            + v[i-1][j][k] + v[i][j+1][k]
                                            + v[i][j][k+1]);
            }
        }
    }
}


void smoothing_iterator3D (double ***u, double ***v, int nx, int ny,
                                 int nz, int iterations)
{
    void smooth3D (double ***u, double ***v, int nx, int ny, int nz);
    void copy_array3D (double ***v, double ***u, int nx, int ny, int nz);
    void swap3D (double ****v_ptr, double ****u_ptr);

    unsigned int iter = 0;

    //printf("Copying v array into u to get correct boundaries ...");
    copy_array3D(u, v, nx, ny, nz);   
    //printf("DONE.\n");

    //printf("Smoothing for %d itearions ...", iterations);
    while (iter < iterations) {
        smooth3D(u, v, nx, ny, nz);
        //swap3D(&v, &u);
        copy_array3D(v, u, nx, ny, nz);   
        // set v = u for next iteration 
        iter++;
    }
    //printf("DONE.\n");
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


void copy_array3D (double ***v, double ***u, int nx, int ny, int nz)
{
    int array_size = nz;
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            memcpy(v[i][j], u[i][j], array_size);
        }
    }
}


void swap3D (double ****v_ptr, double ****u_ptr)
{
    double ***u_tmp = *v_ptr;
    *v_ptr = *u_ptr;
    *u_ptr = u_tmp;
}
