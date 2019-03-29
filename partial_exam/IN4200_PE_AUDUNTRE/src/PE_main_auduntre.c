#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "PE_functions_auduntre.h"


int main (int argc, char **argv)
{
    int maxiter = 10;
    double damping = 0.85;

    if (argc > 1) {
        maxiter = atoi(argv[1]);
        if (argc > 2) {
            damping = atof(argv[2]);
        }
    }

    double start = omp_get_wtime();
    CRS crs_test = read_graph_from_file("web_graphs/8-webpages.txt");
    double end = omp_get_wtime();

    printf("ELAPSED TIME READING GRPAH: %f sec.\n", end - start);
 
    printf("crs.row_ptr: \n");
    for (int i = 0; i < crs_test.len_row_ptr; i++) {
        printf("row_ptr[%d] = %d\n", i, crs_test.row_ptr[i]);
    }

    printf("crs.col_idx - crs.val: \n");
    for (int j = 0; j < crs_test.len_val; j++) {
        printf("col_idx[%d] = %d, val[%d] = %f\n", j, crs_test.col_idx[j],
               j, crs_test.val[j]);
    }

    printf("dangling: \n");
    for (int k = 0; k < crs_test.len_dangling; k++) {
        printf("dangling[%d] = %d\n", k, crs_test.dangling[k]);
    }

    start = omp_get_wtime();
    double *x = PageRank_iterations (crs_test, 1, maxiter, 1.0E-14);
    end = omp_get_wtime();

    printf("ELAPSED TIME PAGERANK ITER: %f sec.\n", end - start);

    for (int i = 0; i < crs_test.len_row_ptr - 1; i++) {
        printf("x[%d] = %f \n", i, x[i]);
    }
    
    free(crs_test.val);
    free(crs_test.col_idx);
    free(crs_test.row_ptr);
    free(crs_test.dangling);
    return 0;
}
