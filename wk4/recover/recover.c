#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

bool is_jpeg_header(unsigned char file_arr[][512], int block_looking, int blocks_count);

int main(int argc, char *argv[])
{
    int jpeg_count = 0;
    int file_size;
    int blocks_count;
    // Check args
    if (argc != 2)
    {
        printf("Please specify the file path\n");
        return 1;
    }
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
    
    blocks_count = file_size / 512; 
    
    uint8_t file_arr[blocks_count][512];
    

    for (int block = 0; block < blocks_count; block++)
    {
        fread(&file_arr[block], 512, 1, inpt);
    }
    
    for (int block_looking = 0; block_looking < blocks_count; block_looking++)
    {
        if (is_jpeg_header(file_arr, block_looking, blocks_count))
        {
            jpeg_count++;
            int image_block_size = 1;
            
            while (!is_jpeg_header(file_arr, (block_looking+image_block_size), blocks_count))
            {
                image_block_size++;
            }
            char file_name[8];
            sprintf(file_name, "%03i.jpg", (jpeg_count - 1));
            FILE *outpt = fopen(file_name, "w");
            for (int printing_block = 0; printing_block < image_block_size; printing_block++)
            {
                for (int printing_char_of_block = 0; printing_char_of_block < 512; printing_char_of_block++)
                {
                    fprintf(outpt, "%c", file_arr[block_looking + printing_block][printing_char_of_block]);
                }
            }
            fclose(outpt);
        }
    }
    
}

bool is_jpeg_header(unsigned char file_arr[][512], int block_looking, int blocks_count)
{
    if (block_looking >= blocks_count)
    {
        return true;
        // no more blocks!
    }
    if (file_arr[block_looking][0] == 0xff && file_arr[block_looking][1] == 0xd8 && file_arr[block_looking][2] == 0xff)
    {
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
