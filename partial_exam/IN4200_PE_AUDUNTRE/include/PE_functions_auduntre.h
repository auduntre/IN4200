/** @file PE_functions_auduntre.h
 *  @brief Functions for ranking webpages using the PageRank algorithm.
 *  
 *  @author Audun Tahina Reitan
 */

#ifndef PE_FUNCTIONS_AUDUNTRE_H
#define PE_FUNCTIONS_AUDUNTRE_H

double ** read_grap_from_file (char *filename);
 
double * PageRank_iterations (int damping, double threshold);


/** @breif Listing out the top n websites.
 *  
 *  @param PE_score_vector The score vector for webpages.
 *  @param top_n The number of top sites to list out.
 *  @return Void  
 */
void top_n_webpages (double *PE_score_vector, int top_n);

#endif
