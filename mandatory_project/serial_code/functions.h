#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef struct
{
    float *image_data_storage;  /* underlaying 1D storage */
    float **image_data;         /* a 2D array of floats */
    int m;                      /* # pixels in vertical-direction */
    int n;                      /* # pixels in horizontal-direction */
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


/** FUNCTION swap_images
 * ---------------------
 * Swap two image pointers.
 *
 * parameters:
 *   - image *u: Pointer to image to be swapped.
 *   - image *v: Pointer to image to be swapped.
 */
void swap_images (image *u, image *v);


/** FUNCTION convert_jpeg_to_image
 * ---------------------.---------
 * Converts an 1D char array of a picture to a 2D float array in 
 * an image struct.  
 *
 * parameters:
 *   - char *image_chars: Pointer to 1D char array of picture. 
 *   - image *u:          Pointer to image to contain 2D float array.
 */
void convert_jpeg_to_image (const unsigned char *image_chars, image *u);


/** FUNCTION convert_jpeg_to_image
 * ------------------------------
 * Converts a 2D float array in an image struct to an 1D char array.
 *
 * parameters:
 *   - char *image_chars: Pointer to 1D char array of picture. 
 *   - image *u:          Pointer to image containig 2D float array.
 */
void convert_image_to_jpeg (const image *u, unsigned char* image_chars);


/** FUNCTION iso_diffusion_denoising
 * ---------------------------------
 * Isotropic diffusion denoising of an image.
 *
 * parameters:
 *   - image *u:     Pointer struct of picture to be denoised
 *   - image *u_bar: Pointer to updated image for each iteration. The final denoised
 *                   picture is in this struct image_data array.
 *   - float kappa:  Constant values used in the Denoising.
 *   - int iters:    Number of iterations.  
 */
void iso_diffusion_denoising (image *u, image *u_bar, float kappa, int iters);

#endif
