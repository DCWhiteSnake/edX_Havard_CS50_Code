#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef uint8_t BYTE;
int BLOCK_SIZE = 512;
/**
 * main - Entry point into the forensic recovery tool
 * @argc: the number of command line arguments
 * @argv: the command line arguments
 * Return - 0 if no error, 1 for incorrect usage.
 */
int main(int argc, char *argv[])
{
    FILE *jpg;
    BYTE *buffer;
    char *curr_jpg;
    int image_no;

    if (argc != 2)
    {
        printf("usage: ./recover IMAGE\n");
        return (1);
    }
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("The file \"%s\" does not exist", argv[1]);
        return (1);
    }

    buffer = calloc(BLOCK_SIZE, sizeof(BYTE));
    curr_jpg = calloc(8, sizeof(char));
    image_no = -1;
    jpg = fopen("temp.jpg", "r");

    // Scan till end of file.
    while (fread(buffer, sizeof(BYTE), BLOCK_SIZE, file) == BLOCK_SIZE)
    {
        // If this is the start of a new jpg image.
        if (buffer[0] == 0xff && buffer[1] == 0xd8 &&
            buffer[2] == 0xff && ((0xf0 & buffer[3]) == 0xe0))
        {
            // if this is the first image
            if (image_no == -1)
            {
                sprintf(curr_jpg, "%03i.jpg", ++image_no);
                jpg = fopen(curr_jpg, "w+");
            }
            // Or else close whatever image is open
            // increase file number by 1, and finally open the file with the new name
            else
            {
                fclose(jpg);
                sprintf(curr_jpg, "%03i.jpg", ++image_no);
                jpg = fopen(curr_jpg, "w+");
            }
            // flush buffer to the new file.
            fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, jpg);
        }cd 
        else
        {
            if (jpg != NULL)
            {
                fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, jpg);
            }
        }
    }
    // Perform memory sanitization
    fclose(jpg);
    fclose(file);
    free(buffer);
    free(curr_jpg);
    return (0);
}