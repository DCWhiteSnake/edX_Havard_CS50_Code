// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);
    uint8_t new_header[44];
    // TODO: Copy header from input file to output file
    fread(&new_header, sizeof(uint8_t), 44, input);
    fwrite(&new_header, sizeof(uint8_t), 44, output);
    // TODO: Read samples from input file and write updated data to output file

    int16_t *sample_buffer = malloc(sizeof(int16_t));
    while (fread(sample_buffer, sizeof(int16_t), 1, input))
    {
        *sample_buffer = *sample_buffer * factor;
        fwrite(sample_buffer, sizeof(int16_t), 1, output);
    }
    free(sample_buffer);
    // Close files
    fclose(input);
    fclose(output);
}
