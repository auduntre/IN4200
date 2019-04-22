/* Serial Main */
#include <stdlib.h>
#include <stdio.h>

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
    unsigned char *image_chars;
    int m, n, c, iters;
    float kappa;
    
    image u, u_bar; 

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

    import_JPEG_file(input_jpeg_filename, &image_chars, &m, &n, &c);
    printf("Importing JPEG pictures successfull\n");

    allocate_image (&u, m, n);
    allocate_image (&u_bar, m, n);
    printf("Allocation of image arrays succsessfull\n");

    convert_jpeg_to_image (image_chars, &u);
    printf("JPEG -> 2D array successfull\n");

    iso_diffusion_denoising (&u, &u_bar, kappa, iters); 
    printf("Denoising succcessfull\n"); 

    convert_image_to_jpeg (&u_bar, image_chars);
    printf("2D array -> JPEG successfull\n");

    export_JPEG_file(output_jpeg_filename, image_chars, m, n, c, 75);
    printf("Export of JPEG successfull\n");

    deallocate_image (&u);
    deallocate_image (&u_bar);
    printf("Deallocation of image arrays succsessfull\n");
    
    return EXIT_SUCCESS;
}

