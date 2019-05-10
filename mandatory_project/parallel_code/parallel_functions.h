#ifndef PARALLEL_FUNCTIONS_H
#define PARALLEL_FUNCTIONS_H

#include "functions.h"

/** FUNCTION iso_diffusion_denoising_parallel
 * ---------------------------------
 * Parallel isotropic diffusion denoising of an image using MPI with
 * a cartesian process gird.
 *
 * parameters:
 *   - image *u:           Pointer struct of this process block of the 
 *                         picture to be denoised.
 *   - image *u_bar:       Pointer to updated image for each iteration. 
 *                         The final denoised block of the picture is 
 *                         in this struct image_data array.
 *   - float kappa:        Constant values used in the Denoising.
 *   - int iters:          Number of iterations.
 *   - MPI_COMM CART_COMM: The cartesian process grid. 
 */
void iso_diffusion_denoising_parallel (image *u, image *u_bar, float kappa, 
                                       int iters, MPI_Comm CART_COMM);

#endif
