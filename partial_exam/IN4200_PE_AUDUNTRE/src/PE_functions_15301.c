#include "PE_functions_15301.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>


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
    
    // First run through file
    while (fscanf(graph_file, "%d %d", &from_node_id, &to_node_id) == 2) {
        if (from_node_id != to_node_id) {
            link_count_col[from_node_id] += 1.0;
            link_count_row[to_node_id] += 1;
            edge_idx++;
        }
    }

    CRS crs = {
        .row_ptr = (int *) malloc ((node_count + 1) * sizeof(int)),
        .col_idx = (int *) malloc (edge_idx * sizeof(int)),
        .val = (double *) malloc (edge_idx * sizeof(double)),
        .len_row_ptr = node_count + 1, 
        .len_val = edge_idx,
        .len_dangling = 0,
    };

    // tmp array with size max number possible for dangling
    int *tmp_dangling = (int *) malloc (node_count * sizeof(int)); 
    for (int i = 1; i < crs.len_row_ptr; i++) {
        crs.row_ptr[i] = crs.row_ptr[i-1] + link_count_row[i-1];
        
        if (link_count_col[i-1] == 0) {
            tmp_dangling[crs.len_dangling] = i-1;
            crs.len_dangling++;
        }
    }

    crs.dangling = (int *) malloc (crs.len_dangling * sizeof(int));
    for (int k = 0; k < crs.len_dangling; k++) {
        crs.dangling[k] = tmp_dangling[k]; 
    }

    for (int j = 0; j < crs.len_val; j++) {
        crs.col_idx[j] = -1;
    } 
    
    rewind(graph_file);
    for (int header_idx = 0; header_idx < FILE_HEADER_LINES; header_idx++) {
        fscanf(graph_file, "%*[^\n]\n");
    }

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


void top_n_webpages (double *PE_score_vector, int len_n, int top_n)
{
    void find_top_n (double *score_vector, double *top_vector, int *top_pos,
                     int len_n, int beg, int end);
    int find_min_idx(double *score_vector, double *min_value, int beg, int end);
    void permution_sort (double *score_vector, int *perm, int beg, int end);

    FILE *output_file = fopen("report/output.txt", "w");

    if (top_n > len_n) {
        printf("len_n should be bigger or equal to top_n\n");
        exit(EXIT_FAILURE);
    }

    double *top_n_vector = (double *) malloc (top_n * sizeof(double));
    int *top_n_pos = (int *) malloc (top_n * sizeof(int));

    int nthreads = omp_get_max_threads();
    int thread_interval = len_n / nthreads;
    int len_top_nt = nthreads * top_n;
    int min_idx;
    int i, j, k;

    double min_value;
    
    double *top_nthreads_vector = (double *) malloc (len_top_nt * sizeof(double));
    int *top_nthreads_pos = (int *) malloc (len_top_nt * sizeof(int));
    int *perm = (int *) malloc (top_n * sizeof(int));

    //NO point in parallelzing small top_n / len_n or small len_n
    if (len_n > 1000000 * nthreads && top_n < thread_interval/10 && nthreads > 1) {
        int start_idx[nthreads + 1];
        
        start_idx[0] = 0;
        for (i = 1; i < nthreads; i++) {
            start_idx[i] = start_idx[i-1] + thread_interval; 
        }
        start_idx[nthreads] = len_n;

        #pragma omp parallel for private(i, j, k, min_value, min_idx)
        for (i = 0; i < nthreads; i++) {
            int k0 = i * top_n;
            k = k0;

            for (j = start_idx[i]; j < start_idx[i] + top_n - 1; j++) {
                top_nthreads_vector[k] = PE_score_vector[j];
                top_nthreads_pos[k] = j;
                k++;
            }

            min_idx = find_min_idx(top_nthreads_vector, &min_value, k0, k);
        
            for (j = start_idx[i] + top_n; j < start_idx[i+1]; j++) {
                if (PE_score_vector[i] > min_value) {
                    top_nthreads_vector[min_idx] = PE_score_vector[j];
                    top_nthreads_pos[min_idx] = j;

                    min_idx = find_min_idx(top_nthreads_vector, &min_value, k0, k);
                }
            }
        }

        find_top_n(top_nthreads_vector, top_n_vector, top_n_pos, len_top_nt, 0, top_n);
    
        for (i = 0; i < top_n; i++) {
            top_n_pos[i] = top_nthreads_pos[top_n_pos[i]];
        }

    }
    else {
        find_top_n(PE_score_vector, top_n_vector, top_n_pos, len_n, 0, top_n);
    }

    permution_sort (top_n_vector, perm, 0, top_n);

    for(int k = top_n - 1; k >= 0; k--) {
        int rank = top_n - k;
        fprintf(output_file, "Rank: %d, Site: %d, PE_score: %.10g\n", 
                rank, top_n_pos[perm[k]], top_n_vector[perm[k]]);
    }

    fclose(output_file);
    free(perm);
    free(top_nthreads_pos);
    free(top_nthreads_vector);
    free(top_n_pos);
    free(top_n_vector);
    return;
}



void find_top_n (double *score_vector, double *top_vector, int *top_pos,
                 int len_n, int beg, int end)
{
    int find_min_idx (double *score_vector, double *min_value, int beg, int end);
    
    double min_value;
    int min_idx;

    for (int i = beg; i < end; i++) {
        top_vector[i] = score_vector[i];
        top_pos[i] = i;        
    }
    
    min_idx = find_min_idx(top_vector, &min_value, beg, end);

    for (int i = end; i < len_n; i++) {
        if (score_vector[i] > min_value) {
            top_vector[min_idx] = score_vector[i];
            top_pos[min_idx] = i;

            min_idx = find_min_idx(top_vector, &min_value, beg, end);
        }
    }
} 


int find_min_idx (double *score_vector, double *min_value, int beg, int end)
{
    int min_idx = beg;
    *min_value = score_vector[beg];

    for (int i = beg; i < end; i++) {
        if (score_vector[i] < *min_value) {
            min_idx = i;
            *min_value = score_vector[i];
        }
    }

    return min_idx;
}


void permution_sort (double *arr, int *perm, int beg, int end)
{
    void sort_perm (double *arr, int *perm, int beg, int end);
    
    // INIT PERM MATRIX
    for (int i = beg; i < end; i++) {
        perm[i] = i;
    }

    sort_perm(arr, perm, beg, end);
}


void sort_perm (double *arr, int *perm, int beg, int end)
{
    void swap (int *a, int *b);

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
        sort_perm(arr, perm, beg, l);
        sort_perm(arr, perm, r, end);
    } 
}


void swap (int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}
