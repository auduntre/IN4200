#ifndef PARALLEL_FUNCTIONS_H
#define PARALLEL_FUNCTIONS_H

#include "functions.h"

void iso_diffusion_denoising_parallel (image *u, image *u_bar, float kappa, 
                                       int iters, MPI_Comm CART_COMM);

#endif
