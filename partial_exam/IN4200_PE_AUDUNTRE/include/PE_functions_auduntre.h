#ifndef PE_FUNCTIONS_AUDUNTRE_H
#define PE_FUNCTIONS_AUDUNTRE_H

#include <string.h>

double ** read_grap_from_file (char *filename);
 
double * PageRank_iterations (int damping, double threshold);

void top_n_webpages (double *PE_score_vector, int top_n);

#endif
