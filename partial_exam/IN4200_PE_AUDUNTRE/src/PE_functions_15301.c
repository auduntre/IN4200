#include "PE_functions_15301.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>


// See header file
CRS read_graph_from_file (char *filename)
{
    FILE *graph_file;
    graph_file = fopen(filename, "r");

    if (!graph_file) {
        printf("File with filname given was not found.\n");
        exit(EXIT_FAILURE);
    }

    int edge_count, node_count; 
    int from_node_id, to_node_id;
    int edge_idx = 0;

    for (int header_idx = 0; header_idx < FILE_HEADER_LINES; header_idx++) {
        // third line contains node and edge count
        if (header_idx == 2) {
            fscanf(graph_file, "%*[^0-9]%d %*[^0-9]%d \n",
                   &node_count, &edge_count);
        }
        else {
            fscanf(graph_file, "%*[^\n]\n");
        }
    }
    
    double *link_count_col = (double *) malloc (node_count * sizeof(double));
    int *link_count_row = (int *) malloc (node_count * sizeof(int));

    for (int i = 0; i < node_count; i++) {
        link_count_col[i] = 0.0;
        link_count_row[i] = 0;
    }
    
    // First run through file find number of inbound and outbound links
    while (fscanf(graph_file, "%d %d", &from_node_id, &to_node_id) == 2) {
        if (from_node_id != to_node_id) {
            link_count_col[from_node_id] += 1.0;
            link_count_row[to_node_id] += 1;
            edge_idx++;
        }
    }

    // Compressed row storage of matrix
    CRS crs = {
        .row_ptr = (int *) malloc ((node_count + 1) * sizeof(int)),
        .col_idx = (int *) malloc (edge_idx * sizeof(int)),
        .val = (double *) malloc (edge_idx * sizeof(double)),
        .len_row_ptr = node_count + 1, 
        .len_val = edge_idx,
        .len_dangling = 0,
    };

    // tmp array with size max number possible for dangling
    // find how many values in each row by its number of inbound links
    // set row_ptr for that row to row_ptr for last row + number of inbound
    int *tmp_dangling = (int *) malloc (node_count * sizeof(int)); 
    for (int i = 1; i < crs.len_row_ptr; i++) {
        crs.row_ptr[i] = crs.row_ptr[i-1] + link_count_row[i-1];
        
        if (link_count_col[i-1] == 0) {
            tmp_dangling[crs.len_dangling] = i-1;
            crs.len_dangling++;
        }
    }

    // Set in the dangling sites
    crs.dangling = (int *) malloc (crs.len_dangling * sizeof(int));
    for (int k = 0; k < crs.len_dangling; k++) {
        crs.dangling[k] = tmp_dangling[k]; 
    }

    // IF column index == -1 it's value has not be filled yet
    for (int j = 0; j < crs.len_val; j++) {
        crs.col_idx[j] = -1;
    } 
    
    // Rewind file and skip header
    rewind(graph_file);
    for (int header_idx = 0; header_idx < FILE_HEADER_LINES; header_idx++) {
        fscanf(graph_file, "%*[^\n]\n");
    }

    // Read in value array == 1/number of outbound from site,  with column indices
    while (fscanf(graph_file, "%d %d", &from_node_id, &to_node_id) == 2) {
        if (from_node_id != to_node_id) {
            for (int j = crs.row_ptr[to_node_id]; j < crs.row_ptr[to_node_id + 1]; j++) {
                if (crs.col_idx[j] == -1) {
                    crs.col_idx[j] = from_node_id;
                    crs.val[j] = 1.0 / link_count_col[from_node_id];
                    break;
                }
            }
        }
    }

    fclose(graph_file);
    free(link_count_col);
    free(link_count_row);
    free(tmp_dangling);    
    return crs;
}
 

// See header file
double * PageRank_iterations (CRS crs, double damping, int maxiter, double threshold)
{
    int node_count = crs.len_row_ptr - 1;

    double *pagerank_score = (double *) malloc (node_count * sizeof(double));
    double *pagerank_score_past = (double *) malloc (node_count * sizeof(double));
    double init_guess = 1.0 / (double) node_count;

    double inf_norm = 1.0;
    double iter_const = 0.0;
    double dangling_score = 0.0;
    double xtmp;
    int iteration_idx = 0;
    int i, k, j;
    
    #pragma omp parallel private(i, j, k, xtmp)
    {
    
    // Initial guess
    #pragma omp for
    for (i = 0; i < crs.len_row_ptr - 1; i++) {
        pagerank_score[i] = init_guess;
    }

    while(iteration_idx < maxiter && inf_norm > threshold) {
        #pragma omp single
        {
            dangling_score = 0.0;
        }
       
        #pragma omp for
        for (i = 0; i < node_count; i++) {
            pagerank_score_past[i] = pagerank_score[i]; 
        }

        #pragma omp for reduction(+: dangling_score)
        for (k = 0; k < crs.len_dangling; k++) {
            dangling_score += pagerank_score_past[crs.dangling[k]];
        }
        
        #pragma omp single
        {
            iter_const = (1.0 - damping + damping * dangling_score) / node_count;
            
            #ifdef VERBOSE_ITERATIONS
            double inf_norm_past = inf_norm;
            #endif 
            
            inf_norm = 0.0;
        }

        // Sparse matrix multiplication
        #pragma omp for reduction(max: inf_norm) schedule(dynamic, 1024)
        for (i = 0; i < node_count; i++) {
            xtmp = 0.0;
            
            for (j = crs.row_ptr[i]; j < crs.row_ptr[i+1]; j++) {
                xtmp += crs.val[j] * pagerank_score_past[crs.col_idx[j]];
            }

            xtmp = iter_const + damping * xtmp;
            inf_norm = fmax(inf_norm, fabs(xtmp - pagerank_score[i]));
            pagerank_score[i] = xtmp;
        }

        #pragma omp single
        {
            iteration_idx++;
            
             #ifdef VERBOSE_ITERATIONS
            double rel_norm = (inf_norm_past - inf_norm) / inf_norm_past;
            printf("iteration: %d, inf_norm = %.8g, relative norm change = %.8g\n", 
                   iteration_idx, inf_norm, rel_norm);
            #endif
        }
    }
    }
    
    if (iteration_idx == maxiter && inf_norm > threshold) {
        printf("DID NOT CONVERGE BELOW THRESHOLD FOR %d ITERATIONS.\n", maxiter);
    }
    else {
        printf("USED %d ITERATIONS TO CONVERGE BELOW THRESHOLD OF %.8G. \n", 
               iteration_idx, threshold);
    }

    free(pagerank_score_past);
    return pagerank_score;
}


// See header file
void top_n_webpages (double *PE_score_vector, int len_n, int top_n)
{
    void permution_sort (double *score_vector, int *perm, int beg, int end);

    FILE *output_file = fopen("report/output.txt", "w");

    if (top_n > len_n) {
        printf("len_n should be bigger or equal to top_n\n");
        exit(EXIT_FAILURE);
    }


    int *perm = (int *) malloc (len_n * sizeof(int));

    // Sort the PE_score_vector by a parallel permutation algorithm 
    permution_sort (PE_score_vector, perm, 0, len_n);

    // List out top n in a file
    for(int k = len_n - 1; k >= len_n - top_n; k--) {
        int rank = len_n - k;
        fprintf(output_file, "Rank: %7d, Site: %7d, PE_score: %.10g\n", 
                rank, perm[k], PE_score_vector[perm[k]]);
    }

    fclose(output_file);
    free(perm);
    return;
}


/** FUNCTION: permutation_sort
 * --------------------------
 *  A parallel quicksort algorithm on a permutation vector.
 *
 *  @param *arr: The vector to be sorted by.
 *  @param *perm: The permutation vector to be sorted.
 *  @param beg: The begining index to sort by. Usally equal 0 for the first.
 *  @param end: The end index to sort by: Usally length of arr for the first.
 *  @return Void
 */
void permution_sort (double *arr, int *perm, int beg, int end)
{
    void sort_perm (double *arr, int *perm, int beg, int end);
    
    // INIT PERM MATRIX
    #pragma omp for
    for (int i = beg; i < end; i++) {
        perm[i] = i;
    }

    // Begin sorting
    #pragma omp parallel
    {
    #pragma omp single nowait 
    {
        sort_perm(arr, perm, beg, end);
    }
    }
}

/** FUNCTION: sort_perm
 * --------------------
 *  Auxillary function for the permutation sort. Sorts in serial when the 
 *  difference between the beginning and end index is lower than min_size.
 *  Elsewise sorts the by a pivot and send the lower and higher values to
 *  be sorted by diffirent threads.
 *
 *  For parameters see permutation sort.
 */
void sort_perm (double *arr, int *perm, int beg, int end)
{
    void swap (int *a, int *b);
    int min_size = 10000;

    if (end > beg + 1) {
        double piv = arr[perm[beg]];
        int l = beg + 1;
        int r = end;


        while (l < r) {
            if (arr[perm[l]] <= piv) {
                l++;
            }
            else {
                swap(&perm[l], &perm[--r]);
            }
        }
        
        swap(&perm[--l], &perm[beg]);
        if (end - beg > min_size) {
            #pragma omp task 
            {
                sort_perm(arr, perm, beg, l);
            }
            #pragma omp task
            {
                sort_perm(arr, perm, r, end);
            }
        }
        else {
           sort_perm(arr, perm, beg, l);
           sort_perm(arr, perm, r, end);
        }
    } 
}


/** FUNCTION: swap
 * ---------------
 *  Swaps two pointers. 
 *
 *  @param *a: Pointer to be swapped.
 *  @param *b: Pointer to be swapped.
 *  @return Void
 */
void swap (int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}
