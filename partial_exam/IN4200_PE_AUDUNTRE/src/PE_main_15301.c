#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#include "PE_functions_15301.h"


int main (int argc, char **argv)
{
    
    double damping = 0.85;
    double threshold = 1E-16;
    char *infilename = "web_graphs/8-webpages.txt"; 
    char *outfilename = "report/top_n.txt";
    int maxiter = 1000;
    int top_n = 5;
    
    if (argc > 1) maxiter = atoi(argv[1]);
    if (argc > 2) damping = atof(argv[2]);
    if (argc > 3) threshold = atof(argv[3]);
    if (argc > 4) top_n = atoi(argv[4]);
    if (argc > 5) infilename = argv[5];
    if (argc > 6) outfilename = argv[6];
              
    double start = omp_get_wtime();
    CRS crs_test = read_graph_from_file(infilename);
    double end = omp_get_wtime();

    printf("ELAPSED TIME READING GRPAH: %f sec.\n", end - start);
    
    #ifdef VERBOSE_MAIN
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
    #endif

    start = omp_get_wtime();
    double *x = PageRank_iterations (crs_test, damping, maxiter, 1.0E-16);
    end = omp_get_wtime();

    printf("ELAPSED TIME PAGERANK ITER: %f sec.\n", end - start);

    #ifdef VERBOSE_MAIN
    for (int i = 0; i < crs_test.len_row_ptr - 1; i++) {
        printf("x[%d] = %.10g \n", i, x[i]);
    }
    #endif

    start = omp_get_wtime();
    top_n_webpages(x, crs_test.len_row_ptr -1, top_n);
    end = omp_get_wtime();
    
    printf("ELAPSED TIME TOP N: %f sec.\n", end - start);
    
    free(crs_test.val);
    free(crs_test.col_idx);
    free(crs_test.row_ptr);
    free(crs_test.dangling);
    return 0;
}
