#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

// Prototypes
bool is_jpeg_header(unsigned char file_arr[][512], int block_looking, int blocks_count);

int main(int argc, char *argv[])
{
    // variables for the no jpegs found, total file size, and numebr of blocks present
    int jpeg_count = 0;
    int file_size;
    int blocks_count;
    // Check args
    if (argc != 2)
    {
        printf("Please specify the file path\n");
        return 1;
    }
    // Open raw data file
    FILE *inpt = fopen(argv[1], "r");
    if (!inpt)
    {
        printf("File empty or not found\n");
        return 1;
    }
    
    // Find the end of the file
    fseek(inpt, 0L, SEEK_END);
    file_size = ftell(inpt);
    rewind(inpt);
    // Calculate the numebr of blocks present
    blocks_count = file_size / 512; 
    // Allocate an unsigned int array for each block
    uint8_t file_arr[blocks_count][512];
    // For each start of block
    for (int block = 0; block < blocks_count; block++)
    {
        // Read the data into the block array
        fread(&file_arr[block], 512, 1, inpt);
    }
    // For each start of block
    for (int block_looking = 0; block_looking < blocks_count; block_looking++)
    {
        // If the block starts with a jpeg header
        if (is_jpeg_header(file_arr, block_looking, blocks_count))
        {
            // Weve found another jpeg
            jpeg_count++;
            int image_block_size = 1;
            // While the next block doesnt start with a jpeg header
            while (!is_jpeg_header(file_arr, (block_looking + image_block_size), blocks_count))
            {
                // The image spans this block too
                image_block_size++;
            }
            // Create the file name and file
            char file_name[8];
            sprintf(file_name, "%03i.jpg", (jpeg_count - 1));
            FILE *outpt = fopen(file_name, "w");
            // For each block we have to print from
            for (int printing_block = 0; printing_block < image_block_size; printing_block++)
            {
                // For each char in this block
                for (int printing_char_of_block = 0; printing_char_of_block < 512; printing_char_of_block++)
                {
                    // Print this char out to the file
                    fprintf(outpt, "%c", file_arr[block_looking + printing_block][printing_char_of_block]);
                }
            }
            // Close the file
            fclose(outpt);
        }
    }
    
}

bool is_jpeg_header(unsigned char file_arr[][512], int block_looking, int blocks_count)
{
    // If the block we're looking at is outside the file
    if (block_looking >= blocks_count)
    {
        // Edge case, no more blocks so the jpeg must have finished
        return true;
        // no more blocks!
    }
    // Jpeg header values 0-2
    if (file_arr[block_looking][0] == 0xff && file_arr[block_looking][1] == 0xd8 && file_arr[block_looking][2] == 0xff)
    {
        // Jpeg header values 3
        if ((int) file_arr[block_looking][3] >= 0xe0 && (int) file_arr[block_looking][3] <= 0xef)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}
