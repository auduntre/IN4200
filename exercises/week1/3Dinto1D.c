#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv)
{
    int n_x = 100;
    int n_y = 150;
    int n_z = 75; 

    int i, j, k;
    int ijk;

    // Memory allocation
    double *A_storage = (double *) malloc (n_x * n_y * n_z * sizeof(double));
    double ***A = (double ***) malloc (n_x * sizeof(double **));
    
    for (i = 0; i < n_x; i++) {
        A[i] = (double **) malloc (n_y * sizeof(double *));

        for (j = 0; j < n_y; j++) {
            A[i][j] = &(A_storage[i*n_y*n_z + j*n_z]);

            // Value assignment
            for (k = 0; k < n_z; k++) {
                A[i][j][k] = i*n_y*n_z + j*n_z + k; 
            }
        }
    }

    for (ijk = 0; ijk < n_x*n_y*n_z; ijk++) {
        i = ijk / (n_y * n_z);
        j = ijk / n_z - i * n_y;
        k = ijk - (i * n_y  + j) * n_z;

        //printf("i = %d, j = %d, k = %d \n", i, j, k);
        if (A_storage[ijk] != ijk) {
            printf("Not correctly allocated\n");
        }
    }

    free(A_storage);

    for (i < 0; i < n_x; i++) {
        free(A[i]);
    }
    free(A);

    return 0;
}
