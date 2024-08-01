// Originally from Jeremie Dequidt and Laure Gonnord
// Modified by Julien Forget

#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include "inout.h"

#define DEBUG false
#define BUF_SIZE 256

int load_image_priv(const char *file_name, unsigned char *image,
		    int exp_height, int exp_width, int pix_size)
{
  FILE *fp;
  unsigned int type;
  int imax, w, h, debug;
  unsigned char buffer[BUF_SIZE];

  /* File opening */
  if ((fp = fopen(file_name, "rb")) == NULL) {
#ifdef DEBUG
    printf("[load_image] Cannot open file %s\n",
           file_name);
#endif
    return 0;
  }
#ifdef DEBUG
  printf("[load_image] File %s opened\n", file_name);
#endif
  /* 1- header */
  if (fgets((char *) buffer, BUF_SIZE, fp) == NULL) {
#ifdef DEBUG
    printf("[load_image] Cannot read file \n");
#endif
    return 0;
  }
  if ((type = buffer[1] - 48) != 6) {
#ifdef DEBUG
    printf("[load_image] Image type is not P6 (P%d instead)\n", type);
#endif
    return 0;
  }
  /* 2- skipping comments */
  do {
    if (fgets((char *) buffer, BUF_SIZE, fp) == NULL) {
#ifdef DEBUG
      printf("[load_image] File read error\n");
#endif
      return 0;
    }
  }
  while (buffer[0] == '#');
  /* 3- Sizes */
  if (sscanf((char *) buffer, "%d %d", &w, &h) == 0) {
#ifdef DEBUG
    printf
      ("[load_image] Cannot read image dimensions\n");
#endif
    return 0;
  }
  if (fgets((char *) buffer, BUF_SIZE, fp) == NULL) {
#ifdef DEBUG
    printf("[load_image] Cannot read intensity\n");
#endif
    return 0;
  }
  if (sscanf((char *) buffer, "%d", &imax) == 0) {
#ifdef DEBUG
    printf
      ("[load_image] Cannot read max intensity\n");
#endif
    return 0;
  }
  if ((w != exp_width) || (h != exp_height) || (imax != MAX_INTENSITY)) {
#ifdef DEBUG
    printf
      ("[load_image] Image parameters do no match requirements\n");
    printf("w=%d,h=%d,imax=%d\n", w, h, imax);
    return 0;
#endif
  }
  if (image == NULL) {
#ifdef DEBUG
    printf("[load_image] Image incorrectly allocated\n");
#endif
    return 0;
  }

  /* Read pixels */
  if ((debug = fread(image, 1, w * h * pix_size, fp)) != (w * h * pix_size)) {
#ifdef DEBUG
    printf
      ("[load_image] Could not read all pixel intensities (only read %d)\n",
       debug);
#endif
    return 0;
  }

  /* Everything ok if we get here */
  fclose(fp);
#ifdef DEBUG
  printf("[load_image] File closed\n");
#endif
  return 1;
}

int load_image(const char *file_name, unsigned char image[HEIGHT][WIDTH][PIX_SIZE]) {
  return (load_image_priv(file_name, ((unsigned char*)image), HEIGHT, WIDTH, PIX_SIZE));
}

/*
  Save a PPM image from a char array
 * file_name: Name of the output file
 * image: array storing the pixel intensities
 * Return: 1 if everything went well, 0 otherwise
*/
int save_image(const char *file_name, unsigned char image[HEIGHT][WIDTH][PIX_SIZE])
{
  FILE *fp;

  /* File opening */
  if ((fp = fopen(file_name, "wb")) == NULL) {
#ifdef DEBUG
    printf("[save_image] Cannot open file %s\n",
	   file_name);
#endif
    return 0;
  }
#ifdef DEBUG
  printf("[save_image] File %s opened for writing\n",
	 file_name);
#endif

  /* Header */
  if (!fprintf
      (fp, "P6\n# %s\n%d %d\n%d\n",
       "Polytech'Lille SE3 - project 2022/23", WIDTH, HEIGHT,
       MAX_INTENSITY)) {
#ifdef DEBUG
    printf
      ("[save_image] Cannot write image parameters\n");
#endif
    return 0;
  }
#ifdef DEBUG
  printf
    ("[save_image] Header = Image %d x %d pixels, max intensity = %d\n",
     WIDTH, HEIGHT, MAX_INTENSITY);
#endif

  /* output in file */
  if (fwrite(image, 1, WIDTH * HEIGHT * PIX_SIZE, fp) != (WIDTH * HEIGHT * PIX_SIZE)) {
#ifdef DEBUG
    printf
      ("[save_image] Cannot write pixel intensities\n");
#endif
    return 0;
  }
  /* end of function */
  fclose(fp);
#ifdef DEBUG
  printf("[save_image] File closed\n");
#endif
  return 1;
}
