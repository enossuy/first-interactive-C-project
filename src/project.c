#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "inout.h"

void copy(unsigned char image[HEIGHT][WIDTH][PIX_SIZE],unsigned char img_copy[HEIGHT][WIDTH][PIX_SIZE]);

void blur_image(unsigned char image[HEIGHT][WIDTH][PIX_SIZE],int neighbouring_pixels, int repition);

int mean_adjacency(int rows, int columns, int rgb_value, int neighbouring_pixels, unsigned char matrix[HEIGHT][WIDTH][PIX_SIZE]);

int min_two_values(int i, int j);

int max_two_values(int i, int j);

int min(int i, int j, int k );

int max(int i, int j, int k );

float hue(float r, float g, float b);

float saturation(float r, float g, float b);

float value(float r, float g, float b);

void rgb_to_hsv(unsigned char image[HEIGHT][WIDTH][PIX_SIZE]);

int h_i(int h);

void hsv_to_rgb(unsigned char image[HEIGHT][WIDTH][PIX_SIZE]);

void monochrom(unsigned char image[HEIGHT][WIDTH][PIX_SIZE],float hue_value);

void mosaic(unsigned char image[HEIGHT][WIDTH][PIX_SIZE], int number);

int main()
{
  unsigned char image[HEIGHT][WIDTH][PIX_SIZE];
    
  int charge = load_image("../images/rose.ppm", image);
  
  if (charge == 1) {
    int val;
    int repition;
    int neighbouring_pixels;
    int hue_value;
    int number=1;
    do {
    printf("Hello !\n");
    printf("Kindly choose an option:\n");
    printf("1. Blur the image\n");
    printf("2. Convert the image from RGB format to HSV format\n");
    printf("3. Convert the image from HSV format to RGB format\n");
    printf("4. Adjust the hue value\n");
    printf("5. Make a mosaic of the image\n");
    printf("Type the corresponding number: ");
    scanf("%d",&val);
    switch(val){
            case 1 :
                printf("Kindly type the number of times you want to blur the image: ");
                scanf("%d",&repition);
                printf("Kindly type the number of neighbouring pixels layers you want to consider: ");
                scanf("%d",&neighbouring_pixels);
                blur_image(image,neighbouring_pixels, repition);
                break;
            case 2  :
                rgb_to_hsv(image);
                break;
            case 3  :
                hsv_to_rgb(image);
                break;
            case 4  :
                printf("Kindly type the hue value: ");
                scanf("%d",&hue_value);
                monochrom(image,hue_value);
                break;
            case 5  :
                while ((number%4)!=0){
                    printf("Kindly type the number of images you want to appear in each row: ");
                    scanf("%d",&number);
                }
                mosaic(image,number);
                break;
                 
            default :
                printf("Kindly type a valid number between 1 and 5\n");
                break;
                
        }
        return 0;
    }while(1);
    int save = save_image("rose.ppm", image);
    if (save == 0)
    printf("Could not load specified file\n");
  }

  return 0;
}


void copy(unsigned char image[HEIGHT][WIDTH][PIX_SIZE],unsigned char img_copy[HEIGHT][WIDTH][PIX_SIZE]){
    for (int i=0;i<HEIGHT;i++)
        for (int j=0;j<WIDTH;j++){
            img_copy[i][j][0]=image[i][j][0];
            img_copy[i][j][1]=image[i][j][1];
            img_copy[i][j][2]=image[i][j][2];
        }
}


void blur_image(unsigned char image[HEIGHT][WIDTH][PIX_SIZE],int neighbouring_pixels, int repition){
    int m=neighbouring_pixels, n=repition;
    for (int l=0;l<n;l++)
        for(int i=0; i<HEIGHT; i++)
            for (int j = 0; j < WIDTH; j++) 
                for (int k=0; k<m; k++){
                    image[i][j][0]=mean_adjacency(i,j,0,m,image);
                    image[i][j][1]=mean_adjacency(i,j,1,m,image);
                    image[i][j][2]=mean_adjacency(i,j,2,m,image);
      
        }
    int save = save_image("rose.ppm", image);
    if (save == 0)
      printf("Could not load specified file\n");
  

}
int mean_adjacency (int rows, int columns, int rgb_value, int neighbouring_pixels, unsigned char matrix[HEIGHT][WIDTH][PIX_SIZE]){
    int pixels_number=0, sum=0, m=neighbouring_pixels, k=rgb_value;
    if (rows==0 && columns==0){              // top left corner 
        for (int p=0;p<2*m;p++)
            for (int q=0;q<2*m;q++){
                sum+=matrix[rows+m-p][columns+m-q][k];
                pixels_number++;
            }
    }
    else if (rows==0 && columns==(WIDTH-1)){     // top right corner 
        for (int p=0;p<2*m;p++)
            for (int q=1;q<2*m+1;q++){
                sum+=matrix[rows+m-p][columns+m-q][k];
                pixels_number++;
            }
    }
    else if (rows==(HEIGHT-1) && columns==0){    // bottom left corner 
        for (int p=1;p<2*m+1;p++)
            for (int q=0;q<2*m;q++){
                sum+=matrix[rows+m-p][columns+m-q][k];
                pixels_number++;
        }
    }
    else if (rows==(HEIGHT-1) && columns==(WIDTH-1)){ // bottom right corner 
        for (int p=1;p<2*m+1;p++)
            for (int q=1;q<2*m+1;q++){
                sum+=matrix[rows+m-p][columns+m-q][k];
                pixels_number++;
            }
    }
    else if (rows==0){                 //top edge
        for (int p=0;p<2*m;p++)
            for (int q=0;q<2*m+1;q++){
                sum+=matrix[rows+m-p][columns+m-q][k];
                pixels_number++;
            }
    }
    else if (columns==(WIDTH-1)){       //right edge
        for (int p=0;p<2*m+1;p++)
            for (int q=1;q<2*m+1;q++){
                sum+=matrix[rows+m-p][columns+m-q][k];
                pixels_number++;
            }
    }
    else if (columns==0){               //left edge
        for (int p=0;p<2*m+1;p++)
            for (int q=0;q<2*m;q++){
                sum+=matrix[rows+m-p][columns+m-q][k];
                pixels_number++;
            }
    }
    else if (rows==(HEIGHT-1)){         //bottom edge
        for (int p=1;p<2*m+1;p++)
            for (int q=0;q<2*m+1;q++){
                sum+=matrix[rows+m-p][columns+m-q][k];
                pixels_number++;
            }
    }
    else {                              //everything else
        for (int p=0;p<2*m+1;p++)
            for (int q=0;q<2*m+1;q++){
                sum+=matrix[rows+m-p][columns+m-q][k];
                pixels_number++;
            }
    }
    return sum/pixels_number;
}
int max_two_values(int i, int j){
    if (i>j) return i;
    else return j;
}
int max(int i, int j, int k ){
    if(i>j) return max_two_values(i,k);
    else return max_two_values(j,k);
}

int min_two_values(int i, int j){
    if (i<j) return i;
    else return j;
}
int min(int i, int j, int k ){
    if(i<j) return min_two_values(i,k);
    else return min_two_values(j,k);
}

float hue(float r, float g, float b){
    float m=min(r,g,b);
    float M=max(r,g,b);
    if      (M==m) return   0;
    else if (M==r) return   60*((g-b)/(M-m))+360;
    else if (M==g) return   60*((b-r)/(M-m))+120;
    else           return   60*((r-g)/M-m)+240;
        
}

float saturation(float r, float g, float b){
    float m=min(r,g,b);
    float M=max(r,g,b);
    if (M==0) return 0;
    else      return   ((M-m)/M)*100;
}

float value(float r, float g, float b){
    return 100*(max(r,g,b)/255);
        
}

void rgb_to_hsv(unsigned char image[HEIGHT][WIDTH][PIX_SIZE]){
    for (int i=0;i<HEIGHT;i++)
        for (int j=0;j<WIDTH;j++){
            float r = image[i][j][0];
            float g = image[i][j][1];
            float b = image[i][j][2];
            image[i][j][0]=hue(r,g,b);
            image[i][j][1]=saturation(r,g,b);
            image[i][j][2]=value(r,g,b);
        }
    int save = save_image("rose.ppm", image);
    if (save == 0)
      printf("Could not load specified file\n");
  

}
int h_i(int h){
    return ((int)(h/60))%6;
}
void hsv_to_rgb(unsigned char image[HEIGHT][WIDTH][PIX_SIZE]){
    for (int i=0;i<HEIGHT;i++)
        for (int j=0;j<WIDTH;j++){
            float h=image[i][j][0];
            float s=image[i][j][1];
            float v=image[i][j][2];
            float f= (h/60)-((int)(h/60));
            float p= (v/100)*(1-(s/100))*255;
            float q= ((v/100)*(1-f*(s/100)))*255;
            float t= ((v/100)*(1-(1-f)*(s/100)))*255;
            if (h_i(h)==0) {
                image[i][j][0]=255*v/100;
                image[i][j][1]=t;
                image[i][j][2]=p;
            }
            else if (h_i(h)==1){
                image[i][j][0]=q;
                image[i][j][1]=255*v/100;
                image[i][j][2]=p;
            }
            else if (h_i(h)==2){
                image[i][j][0]=p;
                image[i][j][1]=255*v/100;
                image[i][j][2]=t;
        
            }
            else if (h_i(h)==3){
                image[i][j][0]=p;
                image[i][j][1]=q;
                image[i][j][2]=255*v/100;
        
            }
            else if (h_i(h)==4) {
                image[i][j][0]=t;
                image[i][j][1]=p;
                image[i][j][2]=255*v/100;
        
            }
            else {
                image[i][j][0]=255*v/100;
                image[i][j][1]=p;
                image[i][j][2]=q;
        
            }
    
        }
      int save = save_image("rose.ppm", image);
    if (save == 0)
      printf("Could not load specified file\n");
  
      
}

void mosaic(unsigned char image[HEIGHT][WIDTH][PIX_SIZE], int number){
    unsigned char mosaic_image[HEIGHT][WIDTH][PIX_SIZE];
    copy(image,mosaic_image);
    int n=number;
    for (int i=0;i<HEIGHT/n;i++)
        for (int j=0;j<WIDTH/n;j++)
            for (int k=0;k<n;k++)
                for (int l=0;l<n;l++){
                    image[i+(HEIGHT*k)/n][j+(WIDTH*l)/n][0]=mosaic_image[i*n][j*n][0];
                    image[i+(HEIGHT*k)/n][j+(WIDTH*l)/n][1]=mosaic_image[i*n][j*n][1];
                    image[i+(HEIGHT*k)/n][j+(WIDTH*l)/n][2]=mosaic_image[i*n][j*n][2];
            
            }
        
    int save = save_image("rose.ppm", image);
    if (save == 0)
      printf("Could not load specified file\n");
  

}

void monochrom(unsigned char image[HEIGHT][WIDTH][PIX_SIZE],float hue_value){
   rgb_to_hsv(image);
   
   for (int i=0;i<HEIGHT;i++)
        for (int j=0;j<WIDTH;j++){
            image[i][j][0]=hue_value;
        }
        
   hsv_to_rgb(image);
   int save = save_image("rose.ppm", image);
    if (save == 0)
      printf("Could not load specified file\n");
  

 }
