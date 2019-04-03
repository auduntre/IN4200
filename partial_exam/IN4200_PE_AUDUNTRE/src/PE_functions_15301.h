/** @file PE_functions_auduntre.h
 *  @brief Functions for ranking webpages using the PageRank algorithm.
 *  
 *  @author Audun Tahina Reitan
 */

#ifndef PE_FUNCTIONS_15301_H
#define PE_FUNCTIONS_15301_H

#define FILE_HEADER_LINES 4


/** STRUCT: compressed_row_storage
 * -------------------------------
 *  The compressed row storage for a matrix containing the information between 
 *  intput links (i) from output links (j), called edges. Also contains the indices 
 *  for the dangling nodes.  
 */
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


/** FUNCTION: read_graph_from_file
 * ------------------------------
 *  Reads int the nodes and edges (and store the dangling indices) from a graph file.
 *  Stores the values of the matrix in a compressed row storage form given by the 
 *  struct CRS.  
 *
 *  @param filename: the filanme (and path) to the graph file.
 *  @return CRS: The struct with the sparse matrix and the indices for the dangling 
 *               nodes.  
 */

CRS read_graph_from_file (char *filename);


/** FUNCTION: PageRank_iterations
 * ------------------------------
 *  Calculates the RageRank score for the nodes given by and using the CRS struct.
 *  The number of sites is given by crs.len_row_ptr - 1.
 *
 *  @param CRS crs: Contains the sparse matrix needed to for calculting each iteration.
 *                  Also constains the indices for the dangling nodes.
 *  @param damping: The dampining effect for each iteration of the computation.
 *                  An usual value is 0.85.
 *  @param maxiter: The max number of iteration to do the compuation for. 
 *  @param threshold: If the inf norm between two iterations is less than this, stop
 *                    computation.
 *  @return Double *: The Pagerank score for each node in a vector. 
 */
double * PageRank_iterations (CRS crs, double damping, int maxiter, double threshold);


/** FUNCTION: top_n_webpages
 * ------------------------- 
 *  Listing and writing the top n websites to the report/output.txt file.
 *  The PE_score_vector function will be sorted by a permutation vector using a 
 *  parallel quick sort algorithm.
 *
 *  @param PE_score_vector: The score vector for webpages.
 *  @param len_n: The length of PE_score_vector.
 *  @param top_n: The number of top sites to list out.
 *  @return Void  
 */
void top_n_webpages (double *PE_score_vector, int len_n, int top_n);

#endif
