/* Functions */
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
    for (int i = 0; i < u->m) {
        free(u->image_data[i]);
    }

    free(u->image_data);
}


void convert_jpeg_to_image (const unsigned char* image_chars, image *u)
{
    return;
}


void convert_image_to_jpeg (const image *u, unsigned char* image_chars)
{
    return;
}


void iso_diffusion_denoising (image *u, image *u_bar, float kappa, int iters)
{
    return;
}
