#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#include "parallel_functions.h"

void iso_diffusion_denoising_parallel (image *u, image *u_bar, float kappa,
                                       int iters, MPI_Comm CART_COMM)
{

    int my_rank, num_procs, up, down, left, right;
    int iteration = 1;
    int direction[2] = {0, 1};
    
    float C = (1 - 4*kappa);
    float *upbuf = (float *) malloc (u_bar->n * sizeof(float));
    float *downbuf = (float *) malloc (u_bar->n * sizeof(float));
    float *leftbuf = (float *) malloc (u_bar->m * sizeof(float));
    float *rightbuf = (float *) malloc (u_bar->m * sizeof(float));
    
    MPI_Status my_status;
    MPI_Request my_request;
    MPI_Comm_rank (CART_COMM, &my_rank);
    MPI_Comm_size (CART_COMM, &num_procs);

    MPI_Cart_shift (CART_COMM, 0, 1, &up, &down);
    MPI_Cart_shift (CART_COMM, 1, 1, &left, &right);

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
        /*
        if (down != MPI_PROC_NULL) {
            MPI_Isend (u->image_data[u->n-1], u->n, MPI_FLOAT, down, 0, 
                       CART_COMM, &my_request);
        }

        if (up != MPI_PROC_NULL) {
            MPI_Recv (upbuf, u->n, MPI_FLOAT, up, 0, CART_COMM, &my_status); 
        }
        */

        iteration++;
    }

    free(upbuf);
    free(downbuf);
    free(leftbuf);
    free(rightbuf);
}
