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
    // For each row
    for (int row = 0; row < height; row++)
    {
        // For each column pixel
        for (int col = 0; col < width; col++)
        {
            // Average r, g, b
            int grey_pixel = (int) round(((float) image[row][col].rgbtRed + (float) image[row][col].rgbtGreen + 
                                          (float) image[row][col].rgbtBlue) / (float) 3);
            // Assign r, g, b
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
    // For each row
    for (int row = 0; row < height + 1; row++)
    {
        // For each column pixel
        for (int col = 0; col < ceil(width / 2); col++)
        {
            // Swap pointers
            swap(&image[row][col].rgbtRed, &image[(row)][(width - (col + 1))].rgbtRed);
            swap(&image[row][col].rgbtGreen, &image[(row)][(width - (col + 1))].rgbtGreen);
            swap(&image[row][col].rgbtBlue, &image[(row)][(width - (col + 1))].rgbtBlue);
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create empty memory for temp array using new struct
    pixels(*new_image)[width] = calloc(height, width * sizeof(pixels));
    if (new_image == NULL)
    {
        fprintf(stderr, "Not enough memory to store image.\n");
        return;
    }
    // For each line
    for (int row = 0; row < height; row++)
    {
        // For each pixel
        for (int col = 0; col < width; col++)
        {
            // Set a counter for the number of pixels looked at
            int pixels_to_average = 9;
            // Set up total counters
            int red_total = 0;
            int green_total = 0;
            int blue_total = 0;
            // For each line above or below
            for (int rows_delta = -1; rows_delta < 2; rows_delta++)
            {
                // For each pixel left or right
                for (int cols_delta = -1; cols_delta < 2; cols_delta++)
                {
                    // If the pixel were looking at is outside the image
                    if ((row + rows_delta) < 0 || (row + rows_delta) >= height || (col + cols_delta) < 0 || (col + cols_delta) >= width)
                    {
                        // Ignore this pixel
                        pixels_to_average--;
                        continue;
                    }
                    else
                    {
                        // Else add the pixel to the counter
                        red_total += (int) image[row + rows_delta][col + cols_delta].rgbtRed;
                        green_total += (int) image[row + rows_delta][col + cols_delta].rgbtGreen;
                        blue_total += (int) image[row + rows_delta][col + cols_delta].rgbtBlue;
                    }
                }
            }
            // Average out the totals
            new_image[row][col].red = (int) round((float) red_total / (float) pixels_to_average);
            new_image[row][col].green = (int) round((float) green_total / (float) pixels_to_average);
            new_image[row][col].blue = (int) round((float) blue_total / (float) pixels_to_average);
        }
    }
    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            // Clamp values and move them to the image array from the temp buffer
            image[r][c].rgbtRed = clampy(new_image[r][c].red);
            image[r][c].rgbtGreen = clampy(new_image[r][c].green);
            image[r][c].rgbtBlue = clampy(new_image[r][c].blue);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Initialise sobel kernels
    int gx[3][3] = {
                    {-1, 0, 1}, 
                    {-2, 0, 2}, 
                    {-1, 0, 1}};
    int gy[3][3] = {
                    {-1, -2, -1}, 
                    {0, 0, 0}, 
                    {1, 2, 1}};
    // Gives us an empty array with INT structs, not uint8_t
    pixels(*new_image)[width] = calloc(height, width * sizeof(pixels));
    if (new_image == NULL)
    {
        fprintf(stderr, "Not enough memory to store image.\n");
        return;
    }
    // For each line
    for (int row = 0; row < height; row++)
    {
        // For each pixel
        for (int col = 0; col < width; col++)
        {
            // Set up empty Gx Gy storage
            int red_x_total = 0;
            int green_x_total = 0;
            int blue_x_total = 0;
            int red_y_total = 0;
            int green_y_total = 0;
            int blue_y_total = 0;
            // For each line above and below
            for (int rows_delta = -1; rows_delta < 2; rows_delta++)
            {
                // For each pixel left and right
                for (int cols_delta = -1; cols_delta < 2; cols_delta++)
                {
                    // If the pixel we are looking at falls outside the image
                    if ((row + rows_delta) < 0 || (row + rows_delta) >= height || (col + cols_delta) < 0 || (col + cols_delta) >= width)
                    {
                        // Treat it as a black border (ignore)
                        continue;
                    }
                    else
                    {
                        // Calculate Gx kernel
                        red_x_total += (int) image[row + rows_delta][col + cols_delta].rgbtRed * gx[cols_delta + 1][rows_delta + 1];
                        green_x_total += (int) image[row + rows_delta][col + cols_delta].rgbtGreen * gx[cols_delta + 1][rows_delta + 1];
                        blue_x_total += (int) image[row + rows_delta][col + cols_delta].rgbtBlue * gx[cols_delta + 1][rows_delta + 1];
                        // Calculate Gy kernel
                        red_y_total += (int) image[row + rows_delta][col + cols_delta].rgbtRed * gy[cols_delta + 1][rows_delta + 1];
                        green_y_total += (int) image[row + rows_delta][col + cols_delta].rgbtGreen * gy[cols_delta + 1][rows_delta + 1];
                        blue_y_total += (int) image[row + rows_delta][col + cols_delta].rgbtBlue * gy[cols_delta + 1][rows_delta + 1];
                    }
                }
            }
            // Clamp values and calculate RMS
            new_image[row][col].red = (int) clampy(round(sqrt(pow(red_x_total, 2) + pow(red_y_total, 2))));
            new_image[row][col].green = (int) clampy(round(sqrt(pow(green_x_total, 2) + pow(green_y_total, 2))));
            new_image[row][col].blue = (int) clampy(round(sqrt(pow(blue_x_total, 2) + pow(blue_y_total, 2))));
        }
    }
    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            // Move values to image array
            image[r][c].rgbtRed = (uint8_t) new_image[r][c].red;
            image[r][c].rgbtGreen = (uint8_t) new_image[r][c].green;
            image[r][c].rgbtBlue = (uint8_t) new_image[r][c].blue;
        }
    }
    // Free malloc
    free(new_image);
    return;
}

void swap(BYTE *a, BYTE *b)
{
    // Swap A and B pointers
    BYTE tmp = *a;
    *a = *b;
    *b = tmp;
}

uint8_t clampy(int a)
{
    // Clamp values to be between 0 and 255
    if (a >= 255) 
    {
        return 255;
    }
    else if (a <= 0) 
    {
        return 0;
    }
    else 
    {
        return (uint8_t) a;
    }
}