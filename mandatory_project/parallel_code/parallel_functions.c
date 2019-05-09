#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#include "parallel_functions.h"

void iso_diffusion_denoising_parallel (image *u, image *u_bar, float kappa,
                                       int iters, MPI_Comm CART_COMM)
{

    int my_rank, num_procs,up, down, left, right;
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

    int M = u->m-1;
    int N = u->n-1;

    // First u_bar is copy of u
    for (int i = 0; i < u_bar->m; i++) {
        for (int j = 0; j < u_bar->n; j++) {
            u_bar->image_data[i][j] = u->image_data[i][j];
        } 
    }

    for (int j = 0; j < u_bar->n; j++) {
        upbuf[j] = u->image_data[0][j];
        downbuf[j] = u->image_data[M][j];
    }

    // Denoising iters number of times
    while (iteration <= iters) {
        swap_images(u, u_bar);
        
        if (down != MPI_PROC_NULL) {
            MPI_Isend (u->image_data[M], u->n, MPI_FLOAT, down, 0, 
                       CART_COMM, &my_request);
        }
        
        if (up != MPI_PROC_NULL) {
            MPI_Recv (upbuf, u->n, MPI_FLOAT, up, 0, CART_COMM, &my_status);

            for (int j = 1; j < N; j++) {
                u_bar->image_data[0][j] =  C * u->image_data[0][j] 
                    + kappa * (upbuf[j] + u->image_data[0][j-1] 
                    + u->image_data[0][j+1] + u->image_data[1][j]);
            } 
        }

        leftbuf[0] = u->image_data[0][0];
        rightbuf[0] = u->image_data[0][N];

        for (int i = 1; i < M; i++) {
            leftbuf[i] = u->image_data[i][0];

            for (int j = 1; j < N; j++) {
                // Smoothing procedure
                u_bar->image_data[i][j] =  C * u->image_data[i][j] 
                    + kappa * (u->image_data[i-1][j] + u->image_data[i][j-1] 
                    + u->image_data[i][j+1] + u->image_data[i+1][j]);
            }
            
            rightbuf[i] = u->image_data[i][N];
        }
        
        leftbuf[M] = u->image_data[M][0];
        rightbuf[M] = u->image_data[M][N];

        if (up != MPI_PROC_NULL) {
            MPI_Isend (u->image_data[0], u->n, MPI_FLOAT, up, 1, 
                       CART_COMM, &my_request);
        }
        
        if (down != MPI_PROC_NULL) {
            MPI_Recv (downbuf, u->n, MPI_FLOAT, down, 1, CART_COMM, 
                      &my_status);

            for (int j = 1; j < N; j++) {
                u_bar->image_data[M][j] =  C * u->image_data[M][j] 
                    + kappa * (u->image_data[M-1][j] 
                    + u->image_data[M][j-1] + u->image_data[M][j+1] 
                    + downbuf[j]);
            } 
        }

        if (left != MPI_PROC_NULL) {
            MPI_Isend (leftbuf, u->m, MPI_FLOAT, left, 2, CART_COMM,
                       &my_request);
        }

        if (right != MPI_PROC_NULL) {
            MPI_Isend (rightbuf, u->m, MPI_FLOAT, right, 3, CART_COMM,
                       &my_request);

            MPI_Recv (rightbuf, u->m, MPI_FLOAT, right, 2, CART_COMM, 
                      &my_status);

            for (int i = 1; i < M; i++) {
                u_bar->image_data[i][N] =  C * u->image_data[i][N] 
                    + kappa * (u->image_data[i-1][N] 
                    + u->image_data[i][N-1] + rightbuf[i] 
                    + u->image_data[i+1][N]);
            }
        }

        if (left != MPI_PROC_NULL) {
            MPI_Recv (leftbuf, u->m, MPI_FLOAT, left, 3, CART_COMM, 
                      &my_status);
            
            for (int i = 1; i < M; i++) {
                u_bar->image_data[i][0] =  C * u->image_data[i][0] 
                    + kappa * (u->image_data[i-1][0] 
                    + leftbuf[i] + u->image_data[i][1] 
                    + u->image_data[i+1][0]);
            }
        }

        // Corners
        u_bar->image_data[0][0] = C * u->image_data[0][0]
            + kappa * (upbuf[0] + leftbuf[0] + u->image_data[0][1]
            + u->image_data[1][0]);

        u_bar->image_data[0][N] = C * u->image_data[0][N]
            + kappa * (upbuf[N] + rightbuf[0] + u->image_data[0][N-1]
            + u->image_data[1][N]);
        
        u_bar->image_data[M][0] = C * u->image_data[M][0]
            + kappa * (downbuf[0] + leftbuf[M] + u->image_data[M][1]
            + u->image_data[M-1][0]);
        
        u_bar->image_data[M][N] = C * u->image_data[M][N]
            + kappa * (downbuf[N] + rightbuf[M] + u->image_data[M][N-1]
            + u->image_data[M-1][N]);
        
        iteration++;
    }

    free(upbuf);
    free(downbuf);
    free(leftbuf);
    free(rightbuf);
}
