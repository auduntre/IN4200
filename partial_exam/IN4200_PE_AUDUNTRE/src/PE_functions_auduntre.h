/** @file PE_functions_auduntre.h
 *  @brief Functions for ranking webpages using the PageRank algorithm.
 *  
 *  @author Audun Tahina Reitan
 */

#ifndef PE_FUNCTIONS_AUDUNTRE_H
#define PE_FUNCTIONS_AUDUNTRE_H

// FIND MIN AND MAX FOR 2 OR 3 VARIABLES
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX3(x, y, z) ((x) > (y)) ? (MAX(x, z)) : (MAX(y, z))
#define MIN3(x, y, z) ((x) < (y)) ? (MIN(x, z)) : (MIN(y, z))

#define FILE_HEADER_LINES 4
#define NON_FOUND 0 

struct compressed_row_storage {
    double *val;
    int *col_idx;
    int *row_ptr;

    int len_row_ptr;
};

typedef struct compressed_row_storage CRS;

CRS read_graph_from_file (char *filename);
 
//double * PageRank_iterations (int damping, double threshold);

/** @breif Listing out the top n websites.
 *  
 *  @param PE_score_vector: The score vector for webpages.
 *  @param top_n: The number of top sites to list out.
 *  @return Void  
 */
//void top_n_webpages (double *PE_score_vector, int top_n);

#endif
