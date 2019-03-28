#include "PE_functions_auduntre.h"

#include <stdio.h>
#include <stdlib.h>
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
    int header_line_idx = 0;
    int eof_handler = 1;

    while (header_line_idx < FILE_HEADER_LINES) {
        // THIRD LINE IN FILE HEADER CONTAINS NODE AND EDGE COUNT
        if (header_line_idx == 2) {
            eof_handler = fscanf(graph_file, "%*[^0-9]%d %*[^0-9]%d \n",
                                 &node_count, &edge_count);
            
            if (eof_handler != 2) {
                printf("COULD NOT FIND NODE AND EDGE COUNT IN FILE.\n");
                exit(EXIT_FAILURE);
            }
        }
        else {
            fscanf(graph_file, "%*[^\n]\n");
        }
        header_line_idx++;
    } 
    
    double *link_count = (double *) malloc (node_count * sizeof(double));
    

    for (int i = 0; i < node_count; i++) {
        link_count[i] = 0.0;
    }
    
    int from_node_id, to_node_id, max_node;
    int edge_idx = 0;
    
    while (eof_handler != EOF && eof_handler != NON_FOUND) {
        eof_handler = fscanf(graph_file, "%d %d", &from_node_id, &to_node_id);

        if (eof_handler > 0) {
            if (edge_idx > 0) {
                max_node = MAX3(from_node_id, to_node_id, max_node);
            }
            else {
                max_node = MAX(from_node_id, to_node_id);
            }

            link_count[from_node_id] += 1.0;
            edge_idx++;
        }
    }

    if (edge_idx != edge_count) {
        printf("EDGE COUNT: %d, EDGE COUNT IN HEADER: %d. MISMATCH!\n", 
               edge_idx, edge_count);
        exit(EXIT_FAILURE);
    }
    
    if (max_node >= node_count) {
        printf("BIGGEST NODE: %d, NODE COUNT IN HEADER %d. MISMATCH!\n",
               max_node, node_count);
        exit(EXIT_FAILURE);
    }

    rewind(graph_file);

    // hfi = header_file_idx
    for (int hfi = 0; hfi < FILE_HEADER_LINES; hfi++) {
        fscanf(graph_file, "%*[^\n]\n");
    }

    CRS crs;
    
    crs.len_row_ptr = node_count + 1;
    crs.row_ptr = (int *) malloc (crs.len_row_ptr * sizeof(int));
    crs.col_idx = (int *) malloc (edge_count * sizeof(int));
    crs.val = (double *) malloc (edge_count * sizeof(double));

    while (eof_handler != EOF && eof_handler != NON_FOUND) {
        eof_handler = fscanf(graph_file, "%d %d", &from_node_id, &to_node_id);
    }

    fclose(graph_file);
    free(link_count);
    
    return crs;
}
 

//double * PageRank_iterations (int damping, double threshold);


//void top_n_webpages (double *PE_score_vector, int top_n)
//{
//    return;
//}


