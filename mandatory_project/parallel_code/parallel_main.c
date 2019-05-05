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
    
    /*------------ Creating cartesian grid ---------------*/
    MPI_Comm GRID_COMM_WORLD;
    int dims[2] = {0, 0};
    int periods[2] = {0, 0};
    int my_coords[2];
    
    MPI_Dims_create(num_procs, 2, dims); // division of processors in grid
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &GRID_COMM_WORLD);
    
    // Killing surplus processes
    if (GRID_COMM_WORLD == MPI_COMM_NULL) {
        MPI_Finalize();
        exit(EXIT_SUCCESS);
    }
    
    MPI_Comm_rank (GRID_COMM_WORLD, &my_rank);
    MPI_Comm_size (GRID_COMM_WORLD, &num_procs);
    MPI_Cart_coords(GRID_COMM_WORLD, my_rank, 2, my_coords); 

    if (my_rank == 0) {
        printf("Number of MPI-processes in the grid is set to %d\n", num_procs);
        printf("Rows of processes = %d\n", dims[0]);
        printf("Columns of processes = %d\n", dims[1]);
    }
    /*-------------------------------------------------------*/
    
    if (my_rank == 0) {
        import_JPEG_file(input_jpeg_filename, &image_chars, &m, &n, &c);
        allocate_image (&whole_image, m, n);
    }
   
    MPI_Bcast (&m, 1, MPI_INT, 0, GRID_COMM_WORLD);
    MPI_Bcast (&n, 1, MPI_INT, 0, GRID_COMM_WORLD);
    
    /* 2D decomposition of the m x n pixels evenly among the MPI processes */ 
    my_m = m / dims[0];
    my_n = n / dims[1];

    // Divind out reminders
    if (my_coords[0] < m % dims[0]) my_m += 1;
    if (my_coords[1] < n % dims[1]) my_n += 1;

    allocate_image (&u, my_m, my_n);
    allocate_image (&u_bar, my_m, my_n);
    printf("Allocation done\n");

    /* each process asks process 0 for a partitioned region */
    /* of image_chars and copy the values into u */

    convert_jpeg_to_image (my_image_chars, &u);
    printf("Local jpeg -> image done\n");
    iso_diffusion_denoising_parallel (&u, &u_bar, kappa, iters);
    printf("Computation done \n");

    /* each process sends its resulting content of u_bar to process 0 */
    /* process 0 receives from each process incoming values and */
    /* copy them into the designated region of struct whole_image */
    
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
