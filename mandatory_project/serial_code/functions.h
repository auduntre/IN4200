#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef struct
{
    float **image_data;  /* a 2D array of floats */
    int m;               /* # pixels in vertical-direction */
    int n;               /* # pixels in horizontal-direction */
} image;


/** FUNCTION allocate_image
 * ------------------------
 *  Allocates the m x n 2D array for image data in the 
 *  image struct (image.image_data) and save the dimensions.
 *
 *  parameters:
 *    - image *u: Pointer to the image struct with the image data array to be 
 *                allocated. Also with struct members m and n to be set to the
 *                dimension of the array.
 *    - int m:    Integer number of rows in the image data array.
 *    - int n:    Integer number of columns in the image data array.
 */
void allocate_image (image *u, int m, int n);


/** FUNCTION deallocate_image
 * --------------------------
 * Deallocates the image data array in a image struct.
 *
 * parameters:
 *   - image *u: Pointer to number sturct with the image data array to be
 *               deallocated.
 */
void deallocate_image (image *u);

void swap_images (image *u, image *v);

void convert_jpeg_to_image (const unsigned char* image_chars, image *u);

void convert_image_to_jpeg (const image *u, unsigned char* image_chars);

void iso_diffusion_denoising (image *u, image *u_bar, float kappa, int iters);

#endif
