/** @file PE_functions_auduntre.h
 *  @brief Functions for ranking webpages using the PageRank algorithm.
 *  
 *  @author Audun Tahina Reitan
 */

#ifndef PE_FUNCTIONS_15301_H
#define PE_FUNCTIONS_15301_H

#define FILE_HEADER_LINES 4

struct compressed_row_storage {
    double *val;
    int *col_idx;
    int *row_ptr;

    int *dangling;

    int len_dangling;
    int len_row_ptr;
    int len_val;
};

typedef struct compressed_row_storage CRS;


CRS read_graph_from_file (char *filename);

/** FUNCTION: PageRank_iterations
 * ------------------------------
 *  Calculates the RageRank score for the sites given by the CRS struct. The
 *  number of sites is given by crs.len_row_ptr - 1.
 *
 *  @param CRS crs: Contains the sparse matrix needed to for calculting each iteration.
 *                  Also contains the  
 */
double * PageRank_iterations (CRS crs, double damping, int maxiter, double threshold);


/** FUNCTION: top_n_webpages
 * ------------------------- 
 *  Listing and writing the top n websites to the report/output.txt file.
 *  The PE_score_vector function will be sorted after running this function.
 *
 *  @param PE_score_vector: The score vector for webpages.
 *  @param len_n: The length of PE_score_vector.
 *  @param top_n: The number of top sites to list out.
 *  @return Void  
 */
void top_n_webpages (double *PE_score_vector, int len_n, int top_n);

#endif
