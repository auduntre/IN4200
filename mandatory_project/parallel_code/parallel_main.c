/* Parallel Main */
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#include "parallel_functions.h"
#include "functions.h"

/* declarations of functions import_JPEG_file and export_JPEG_file */
void import_JPEG_file (const char *filename, unsigned char **image_chars,
                       int *image_height, int *image_width,
                       int *num_components);

void export_JPEG_file (const char *filename, const unsigned char *image_chars,
                       int image_height, int image_width,
                       int num_components, int quality);


int main(int argc, char **argv)
{
    char *input_jpeg_filename, *output_jpeg_filename;
    unsigned char *image_chars, *my_image_chars;
    int m, n, c, iters;
    int my_m, my_n, my_rank, num_procs;
    float kappa;
    
    image u, u_bar, whole_image;

    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size (MPI_COMM_WORLD, &num_procs);
    
    /* read from command line: kappa, iters, input_jpeg_filename, output_jpeg_filename */
    if (argc >= 5) {
        kappa = atof(argv[1]);
        iters = atoi(argv[2]);
        input_jpeg_filename = argv[3];
        output_jpeg_filename = argv[4];
    }
    else {
        printf("Too few command line arguments given.\n");
        exit(EXIT_FAILURE);
    }
    
    if (my_rank == 0) {
        import_JPEG_file(input_jpeg_filename, &image_chars, &m, &n, &c);
        allocate_image (&whole_image, m, n);
    }
   
    MPI_Bcast (&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast (&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
   
    /* 2D decomposition of the m x n pixels evenly among the MPI processes */
    my_m = m / num_procs;
    my_n = n;

    allocate_image (&u, my_m, my_n);
    allocate_image (&u_bar, my_m, my_n);
    printf("Allocation done\n");

    /* each process asks process 0 for a partitioned region */
    /* of image_chars and copy the values into u */
    int partition_sizes[num_procs];
    int displacements[num_procs];
    int reminder = m % num_procs;

    displacements[0] = 0;
    
    for (int i = 0; i < num_procs; i++) {
        partition_sizes[i] = n * m / num_procs - 1;
        
        if (reminder > 0) {
            partition_sizes[i] += 1;
            reminder--;
        }

        if (i < num_procs-1) {
            displacements[i+1] = displacements[i] + partition_sizes[i];
        }
    }
    printf("Part and displ done\n"); 

    printf("Dimension MxN = %d x %d = %d\n", m, n, m*n);
    
    for (int i = 0; i < num_procs; i++) {
        printf("MY_RANK %d: partition_sizes[%d] = %d\n", my_rank, i, partition_sizes[i]);
        printf("MY_RANK %d: displacements[%d] = %d\n", my_rank, i, displacements[i]);
    }

    if (my_rank < m % num_procs) {
        MPI_Scatterv (image_chars, partition_sizes, displacements, MPI_CHAR,
                      my_image_chars, my_m*my_n + 1, MPI_CHAR, 0, MPI_COMM_WORLD);
    }
    else {
        MPI_Scatterv (image_chars, partition_sizes, displacements, MPI_CHAR,
                      my_image_chars, my_m*my_n, MPI_CHAR, 0, MPI_COMM_WORLD);
    } 
    printf("Scattering done\n");

    convert_jpeg_to_image (my_image_chars, &u);
    printf("Local jpeg -> image done\n");
    iso_diffusion_denoising_parallel (&u, &u_bar, kappa, iters);
    printf("Computation done \n");

    /* each process sends its resulting content of u_bar to process 0 */
    /* process 0 receives from each process incoming values and */
    /* copy them into the designated region of struct whole_image */
    MPI_Gatherv (u_bar.image_data_storage, u_bar.m*u_bar.n, MPI_FLOAT,
                 whole_image.image_data_storage, partition_sizes, 
                 displacements, MPI_FLOAT, 0, MPI_COMM_WORLD);
 
    if (my_rank == 0) {
        convert_image_to_jpeg(&whole_image, image_chars);
        export_JPEG_file(output_jpeg_filename, image_chars, m, n, c, 75);
        deallocate_image (&whole_image);
    }
 
    deallocate_image (&u);
    deallocate_image (&u_bar);

    MPI_Finalize (); 
    return 0;
}
