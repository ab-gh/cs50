#include "helpers.h"
#include <stdio.h>
#include <math.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

// Prototypes
void swap(BYTE *a, BYTE *b);

typedef struct
{
    int red;
    int blue;
    int green;
} pixels;

pixels pixel;

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            int grey_pixel = (image[row][col].rgbtRed + image[row][col].rgbtGreen + image[row][col].rgbtBlue) / 3;
            // printf("%i\n", grey_pixel);
            image[row][col].rgbtRed = grey_pixel;
            image[row][col].rgbtGreen = grey_pixel;
            image[row][col].rgbtBlue = grey_pixel;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int row = 0; row < height+1; row++)
    {
        for (int col = 0; col < ceil(width/2); col++)
        {
            swap(&image[row][col].rgbtRed, &image[(row)][(width-(col+1))].rgbtRed);
            swap(&image[row][col].rgbtGreen, &image[(row)][(width-(col+1))].rgbtGreen);
            swap(&image[row][col].rgbtBlue, &image[(row)][(width-(col+1))].rgbtBlue);
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*new_image)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (image == NULL)
    {
        fprintf(stderr, "Not enough memory to store image.\n");
        return;
    }
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            int pixels_to_average = 9;
            
            int red_total = 0;
            int green_total = 0;
            int blue_total = 0;

            for (int rows_delta = -1; rows_delta < 2; rows_delta++)
            {
                for (int cols_delta = -1; cols_delta < 2; cols_delta++)
                {
                    if ((row+rows_delta) < 0 || (row+rows_delta) > height || (col+cols_delta) < 0 || (col+cols_delta) > width)
                    {
                        pixels_to_average--;
                        continue;
                    }
                    else
                    {
                        red_total += image[row+rows_delta][col+cols_delta].rgbtRed;
                        green_total += image[row+rows_delta][col+cols_delta].rgbtGreen;
                        blue_total += image[row+rows_delta][col+cols_delta].rgbtBlue;
                    }
                }
            }
            new_image[row][col].rgbtRed = red_total / pixels_to_average;
            new_image[row][col].rgbtGreen = green_total / pixels_to_average;
            new_image[row][col].rgbtBlue = blue_total / pixels_to_average;
        }
    }
    
    image = *new_image;
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}

void swap(BYTE *a, BYTE *b)
{
    BYTE tmp = *a;
    *a = *b;
    *b = tmp;
}