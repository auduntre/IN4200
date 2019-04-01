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

    // Initial guess
    for (int i = 0; i < crs.len_row_ptr - 1; i++) {
        pagerank_score[i] = init_guess;
    }

    double inf_norm = 1.0;
    int iteration_idx = 0;
    int i, k;

    while(iteration_idx < maxiter && inf_norm > threshold) {
        #pragma omp parallel for private(i)
        for (i = 0; i < node_count; i++) {
            pagerank_score_past[i] = pagerank_score[i]; 
        }

        double dangling_score = 0.0;
        for (k = 0; k < crs.len_dangling; k++) {
            dangling_score += pagerank_score_past[crs.dangling[k]];
        }
        double iter_const = (1.0 - damping + damping * dangling_score) / node_count;
        
        #ifdef VERBOSE_ITERATIONS
        double inf_norm_past = inf_norm;
        #endif 
        
        inf_norm = 0.0;

        #pragma omp parallel for private(i) reduction(max: inf_norm) 
        for (i = 0; i < node_count; i++) {
            double xtmp = 0.0;
            
            for (int j = crs.row_ptr[i]; j < crs.row_ptr[i+1]; j++) {
                xtmp += crs.val[j] * pagerank_score_past[crs.col_idx[j]];
            }

            xtmp = iter_const + damping * xtmp;
            double tmp_norm = fabs(xtmp - pagerank_score[i]);
            inf_norm = fmax(tmp_norm, inf_norm);
            pagerank_score[i] = xtmp;
        }

        iteration_idx++;
        #ifdef VERBOSE_ITERATIONS
        double rel_norm = (inf_norm_past - inf_norm) / inf_norm_past;
        printf("iteration: %d, inf_norm = %.8g, relative norm change = %.8g\n", 
               iteration_idx, inf_norm, rel_norm);
        #endif
    }

    if (iteration_idx == maxiter && inf_norm < threshold) {
        printf("DID NOT CONVERGE BELOW THRESHOLD FOR %d ITERATIONS.\n", maxiter);
    }
    else {
        printf("USED %d ITERATIONS TO CONVERGE BELOW THRESHOLD OF %.8G. \n", 
               iteration_idx, threshold);
    }

    free(pagerank_score_past);
    return pagerank_score;
}


void top_n_webpages (double *PE_score_vector, int top_n)
{
    return;
}


