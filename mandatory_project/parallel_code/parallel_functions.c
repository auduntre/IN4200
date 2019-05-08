#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#include "parallel_functions.h"

void iso_diffusion_denoising_parallel (image *u, image *u_bar, float kappa, int iters)
{
    int my_rank, num_procs;
    int iteration = 1;
    float C = (1 - 4*kappa);

    MPI_Request my_request;
    MPI_Status my_status;
    
    MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size (MPI_COMM_WORLD, &num_procs);

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

    float *top_buffer = (float *) malloc (u->n * sizeof(float));
    float *bottom_buffer = (float *) malloc (u->n * sizeof(float));

    // Denoising iters number of times
    while (iteration <= iters) {
        swap_images(u, u_bar);

        // Sending bottom row to rank + 1
        if (my_rank < num_procs - 1) {
            MPI_Isend (&(u->image_data[u->m - 1][0]), u->n, MPI_FLOAT, 
                      my_rank + 1, 0, MPI_COMM_WORLD, &my_request);
        }

        // Smoothing top row with buffer from rank - 1
        if (my_rank > 0) {
            MPI_Recv (top_buffer, u->n, MPI_FLOAT, my_rank - 1,
                     0, MPI_COMM_WORLD, &my_status);
            
            for (int j = 1; j < u_bar->n-1; j++) {
                u_bar->image_data[0][j] = C * u->image_data[0][j]
                    + kappa * (top_buffer[j] + u->image_data[0][j-1]
                    + u->image_data[0][j+1] + u->image_data[1][j]);
            } 
        }

        // Smoothing procedure
        for (int i = 1; i < u_bar->m-1; i++) {
            for (int j = 1; j < u_bar->n-1; j++) {
                u_bar->image_data[i][j] =  C * u->image_data[i][j] 
                    + kappa * (u->image_data[i-1][j] + u->image_data[i][j-1] 
                    + u->image_data[i][j+1] + u->image_data[i+1][j]);
            }
        }    
        iteration++;

        // Sending top row to rank - 1
        if (my_rank > 0) {
            MPI_Isend (&(u->image_data[0][0]), u->n, MPI_FLOAT, my_rank - 1,
                       1, MPI_COMM_WORLD, &my_request);
        }

        // Smoothing bottom row with buffer from rank + 1
        if (my_rank < num_procs - 1) {
            MPI_Recv (bottom_buffer, u->n, MPI_FLOAT, my_rank + 1,
                      1, MPI_COMM_WORLD, &my_status);

            for (int j = 1; j < u_bar->n-1; j++) {
                u_bar->image_data[u->m-1][j] = C * u->image_data[u->m-1][j]
                    + kappa * (u->image_data[u->m-2][j] 
                    + u->image_data[u->m-1][j-1] + u->image_data[u->m-1][j+1]
                    + bottom_buffer[j]);
            }
           
        }
    }

    free(top_buffer);
    free(bottom_buffer);
}
