#include "PE_functions_auduntre.h"

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// FIND MIN AND MAX FOR 2 OR 3 VARIABLES
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX3(x, y, z) ((x) > (y)) ? (MAX(x, z)) : (MAX(y, z))
#define MIN3(x, y, z) ((x) < (y)) ? (MIN(x, z)) : (MIN(y, z))

#define FILE_HEADER_LINES 4
#define NON_FOUND 0 


CRS read_graph_from_file (char *filename)
{
    FILE *graph_file;
    graph_file = fopen(filename, "r");

    if (!graph_file) {
        printf("File with filname given was not found.\n");
        exit(EXIT_FAILURE);
    }

    int from_node_id, to_node_id;
    int min_node, max_node;

    int eof_handler = 1;
    int link_count = 0;
    int header_line_idx = 0;
    
    while (header_line_idx < FILE_HEADER_LINES) {
        fscanf(graph_file, "%*[^\n]\n");
        header_line_idx++;
    } 
    
    while (eof_handler != EOF && eof_handler != NON_FOUND) {
        eof_handler = fscanf(graph_file, "%d %d", &from_node_id, &to_node_id);

        if (eof_handler > 0) {
            if (link_count > 0) {
                min_node = MIN3(from_node_id, to_node_id, min_node);
                max_node = MAX3(from_node_id, to_node_id, max_node);
            }
            else {
                min_node = MIN(from_node_id, to_node_id);
                max_node = MAX(from_node_id, to_node_id);
            }
            
            link_count++;
        }
    }

    int node_count = max_node - min_node + 1; 

    printf("MIN NODE: %d, MAX NODE: %d, N NODES: %d\n", 
            min_node, max_node, node_count);
    printf("LINK COUNT = %d\n", link_count);

    rewind(graph_file);

    fclose(graph_file);

    double *a = (double *) malloc (sizeof(double));
    int *b = (int *) malloc (sizeof(int));
    int *c = (int *) malloc (sizeof(int));

    a[0] = 5.0; b[0] = 13; c[0] = 24;

    CRS crs = {a, b, c};
    return crs;
}
 

//double * PageRank_iterations (int damping, double threshold);


//void top_n_webpages (double *PE_score_vector, int top_n)
//{
//    return;
//}


