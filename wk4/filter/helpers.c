#include "helpers.h"
#include <stdio.h>
#include <math.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Prototypes
void swap(BYTE *a, BYTE *b);
uint8_t clampy(int a);

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
            int grey_pixel = (int) round(((float) image[row][col].rgbtRed + (float) image[row][col].rgbtGreen + (float) image[row][col].rgbtBlue) / (float) 3);
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
    if (new_image == NULL)
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
            new_image[row][col].rgbtRed = (int) round((float) red_total / (float) pixels_to_average);
            new_image[row][col].rgbtGreen = (int) round((float) green_total / (float) pixels_to_average);
            new_image[row][col].rgbtBlue = (int) round((float) blue_total / (float) pixels_to_average);
        }
    }
    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            image[r][c].rgbtRed = new_image[r][c].rgbtRed;
            image[r][c].rgbtGreen = new_image[r][c].rgbtGreen;
            image[r][c].rgbtBlue = new_image[r][c].rgbtBlue;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // edge kernels
    //int gx[3][3] = {{-1, 0, 1},
    //                {-2, 0, 2},
    //                {-1, 0, 1}};
    //int gy[3][3] = {{-1, -2, -1},
    //                {0, 0, 0},
    //                {1, 2, 1}};
    //                
                    
    int gx[3][3] = {{-1, 0, 1},
                    {-2, 0, 2},
                    {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1},
                    {0, 0, 0},
                    {1, 2, 1}};
    RGBTRIPLE(*new_image)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (new_image == NULL)
    {
        fprintf(stderr, "Not enough memory to store image.\n");
        return;
    }
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            uint8_t red_x_total = 0;
            uint8_t green_x_total = 0;
            uint8_t blue_x_total = 0;
            uint8_t red_y_total = 0;
            uint8_t green_y_total = 0;
            uint8_t blue_y_total = 0;

            for (int rows_delta = -1; rows_delta < 2; rows_delta++)
            {
                for (int cols_delta = -1; cols_delta < 2; cols_delta++)
                {
                    if ((row+rows_delta) < 0 || (row+rows_delta) >= height || (col+cols_delta) < 0 || (col+cols_delta) >= width)
                    {
                        continue;
                    }
                    else
                    {
                        red_x_total += clampy((int) image[row+rows_delta][col+cols_delta].rgbtRed * gx[cols_delta+1][rows_delta+1]);
                        green_x_total += clampy((int) image[row+rows_delta][col+cols_delta].rgbtGreen * gx[cols_delta+1][rows_delta+1]);
                        blue_x_total += clampy((int) image[row+rows_delta][col+cols_delta].rgbtBlue * gx[cols_delta+1][rows_delta+1]);

                        red_y_total += clampy((int) image[row+rows_delta][col+cols_delta].rgbtRed * gy[cols_delta+1][rows_delta+1]);
                        green_y_total += clampy((int) image[row+rows_delta][col+cols_delta].rgbtGreen * gy[cols_delta+1][rows_delta+1]);
                        blue_y_total += clampy((int) image[row+rows_delta][col+cols_delta].rgbtBlue * gy[cols_delta+1][rows_delta+1]);
                    }
                }
            }
            new_image[row][col].rgbtRed = ((pow(red_x_total, 2) + pow(red_y_total, 2)) <= 0) ? 0 : round(sqrt(pow(red_x_total, 2) + pow(red_y_total, 2)));
            new_image[row][col].rgbtGreen = ((pow(green_x_total, 2) + pow(green_y_total, 2)) <= 0) ? 0 : round(sqrt(pow(green_x_total, 2) + pow(green_y_total, 2)));
            new_image[row][col].rgbtBlue = ((pow(blue_x_total, 2) + pow(blue_y_total, 2)) <= 0) ? 0 : round(sqrt(pow(blue_x_total, 2) + pow(blue_y_total, 2)));
        }
    }
    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            image[r][c].rgbtRed = (new_image[r][c].rgbtRed > 255) ? 255 : new_image[r][c].rgbtRed;
            image[r][c].rgbtGreen = (new_image[r][c].rgbtGreen > 255) ? 255 : new_image[r][c].rgbtGreen;
            image[r][c].rgbtBlue = (new_image[r][c].rgbtBlue > 255) ? 255 : new_image[r][c].rgbtBlue;
        }
    }
    free(new_image);
    return;
}

void swap(BYTE *a, BYTE *b)
{
    BYTE tmp = *a;
    *a = *b;
    *b = tmp;
}

uint8_t clampy(int a)
{
    if (a >= 255) return 255;
    else if (a <=0) return 0;
    else return (uint8_t) a;
}