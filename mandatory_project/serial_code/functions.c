/** FILE functions.c
 * -----------------
 *  Implementation of the functions from the functions.h header file.
 *  See the header file for program documentation.
 */
#include <stdlib.h>
#include <stdio.h>

#include "functions.h"


void allocate_image (image *u, int m, int n)
{
    u->m = m;
    u->n = n;

    u->image_data = (float **) malloc (u->m * sizeof(float *));
    for (int i = 0; i < u->m; i++) {
        u->image_data[i] = (float *) malloc(u->n * sizeof(float));
    }
}


void deallocate_image (image *u)
{
    for (int i = 0; i < u->m; i++) {
        free(u->image_data[i]);
    }
    free(u->image_data);
}


void swap_images (image *u, image *v)
{
    image tmp_image = *u; 
    *u = *v;
    *v = tmp_image;
}


void convert_jpeg_to_image (const unsigned char *image_chars, image *u)
{
    int imgc_idx = 0;   // Index for iterating through imag_chars

    for (int i = 0; i < u->m; i++) {
        for (int j = 0; j < u->n; j++) {
            u->image_data[i][j] = (float) image_chars[imgc_idx++];
        }
    }
}


void convert_image_to_jpeg (const image *u, unsigned char *image_chars)
{
    int imgc_idx = 0;  // Index for iterating through imag_chars

    for (int i = 0; i < u->m; i++) {
        for (int j = 0; j < u->n; j++) {
            image_chars[imgc_idx++] = (char) u->image_data[i][j]; 
        }
    }
}


void iso_diffusion_denoising (image *u, image *u_bar, float kappa, int iters)
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

