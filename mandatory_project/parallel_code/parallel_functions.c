#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#include "parallel_functions.h"

void iso_diffusion_denoising_parallel (image *u, image *u_bar, float kappa, int iters)
{
    int iteration = 1;
    float C = (1 - 4*kappa);

    if (u_bar->m != u->m || u_bar->n != u->n) {
        printf("Dimensions of images data array doesnt match\n");
        exit(EXIT_FAILURE);
    }

    // First u_bar is copy of u
    for (int i = 0; i < u_bar->m; i++) {
        for (int j = 0; j < u_bar->n; j++) {
            u_bar->image_data[i][j] = u->image_data[i][j];
        }
    }

    // Denoising iters number of times
    while (iteration <= iters) {
        swap_images(u, u_bar);

        for (int i = 1; i < u_bar->m-1; i++) {
            for (int j = 1; j < u_bar->n-1; j++) {
                // Smoothing procedure
                u_bar->image_data[i][j] =  C * u->image_data[i][j] 
                    + kappa * (u->image_data[i-1][j] + u->image_data[i][j-1] 
                    + u->image_data[i][j+1] + u->image_data[i+1][j]);
            }
        }    
        iteration++;
    }
}
