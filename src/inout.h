// Originally from Jeremie Dequidt and Laure Gonnord
// Modified by Julien Forget

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_INTENSITY 255
#define HEIGHT 600
#define WIDTH 800
#define PIX_SIZE 3

/*********************************************************
 * Input/output images
 *********************************************************
 */


/*
  Load a PPM image into a char array
 * - Parameters : 
 *		file_name: Name of the input file
 *		image: array where to store the pixel intensities
 * - Return value : 1 if everything went well, 0 otherwise
 */
int load_image(const char *file_name, unsigned char image[HEIGHT][WIDTH][PIX_SIZE]);

/*
  Save a PPM image from a char array
 * file_name: Name of the output file
 * image: array storing the pixel intensities
 * Return: 1 if everything went well, 0 otherwise
*/
int save_image(const char *file_name, unsigned char image[HEIGHT][WIDTH][PIX_SIZE]);
