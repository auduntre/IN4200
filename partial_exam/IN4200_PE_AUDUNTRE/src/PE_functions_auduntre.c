#include "PE_functions_auduntre.h"

#include <stdio.h>
#include <stdlib.h>

//double ** read_grap_from_file (char *filename);
 

//double * PageRank_iterations (int damping, double threshold);


void top_n_webpages (double *PE_score_vector, int top_n)
{
    for (int i = 0; i < top_n) {
        printf("%i. Site with score: %f", i+1, PE_score_vector[i]);
    }
}


