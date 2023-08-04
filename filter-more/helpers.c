#include <math.h>
#include "helpers.h"

#define BYTE_MAX 255

RGBTRIPLE average_rgb(int width, RGBTRIPLE neighbours[width]);
RGBTRIPLE Calculate_Gx(int size, RGBTRIPLE values[size][size]);
RGBTRIPLE Calculate_Gy(int size, RGBTRIPLE values[size][size]);
RGBTRIPLE Calculate_Resultant(RGBTRIPLE vertical_comp, RGBTRIPLE horizontal_comp);
int my_rounder(double real_value);
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // RGBTRIPLE curr_triple;
    int curr_average;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE curr_triple = image[i][j];
            curr_average = my_rounder(((double)(curr_triple.rgbtRed + curr_triple.rgbtBlue + curr_triple.rgbtGreen)) / 3.0);

            image[i][j].rgbtRed = curr_average;
            image[i][j].rgbtGreen = curr_average;
            image[i][j].rgbtBlue = curr_average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int middle = my_rounder(width / 2.0);
    for (int scanline = 0; scanline < height; scanline++)
    {
        for (int pixel = 0; pixel < width; pixel++)
        {
            // if we are at the center, then there's nothing to swap with again
            if (pixel == middle)
            {
                break;
            }
            // swap
            RGBTRIPLE temp = image[scanline][pixel];
            image[scanline][pixel] = image[scanline][width - pixel - 1];
            image[scanline][width - pixel - 1] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int h, w, i, j, sum[3], sum_blue, count;
    RGBTRIPLE temp[height][width];

    // Copy image to temporary buffer.
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    // Iterate over each pixel in the image.
    for (h = 0; h < height; h++)
    {
        for (w = 0; w < width; w++)
        {
            sum[0] = sum[1] = sum[2] = count = 0;

            // offset the current pixel height and width then sum
            for (int h_offset = -1; h_offset <= 1; h_offset++)
            {
                for (int w_offset = -1; w_offset <= 1; w_offset++)
                {
                    // if we are within the image boundaries the sum up the values
                    if (h + h_offset < height && h + h_offset >= 0)
                    {
                        if (w + w_offset >= 0 && w + w_offset < width)
                        {
                            sum[0] += temp[h + h_offset][w + w_offset].rgbtRed;
                            sum[1] += temp[h + h_offset][w + w_offset].rgbtGreen;
                            sum[2] += temp[h + h_offset][w + w_offset].rgbtBlue;
                            count++;
                        }
                    }
                }
            }
            // Compute average of neighboring pixels
            image[h][w].rgbtRed = round((float)sum[0] / count);
            image[h][w].rgbtGreen = round((float)sum[1] / count);
            image[h][w].rgbtBlue = round((float)sum[2] / count);
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int h, w, r_x, g_x, b_x, r_y, g_y, b_y;
    int gr, gg, gb;

    // Create the Gx and Gy kernels
    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    RGBTRIPLE image_copy[height][width];

    // Apply the sobel filter to each pixel.
    for (h = 0; h < height; h++)
    {
        for (w = 0; w < width; w++)
        {
            r_x = g_x = b_x = r_y = g_y = b_y = 0;

            // Perform weighted sum per pixel
            for (int h_offset = -1; h_offset <= 1; h_offset++)
            {
                for (int w_offset = -1; w_offset <= 1; w_offset++)
                {
                    // if we are within the image boundaries the sum up the values
                    if (h + h_offset < height && h + h_offset >= 0 &&
                        w + w_offset >= 0 && w + w_offset < width)
                    {
                        r_x += gx[h_offset + 1][w_offset + 1] * image[h + h_offset][w + w_offset].rgbtRed;
                        g_x += gx[h_offset + 1][w_offset + 1] * image[h + h_offset][w + w_offset].rgbtGreen;
                        b_x += gx[h_offset + 1][w_offset + 1] * image[h + h_offset][w + w_offset].rgbtBlue;
                        r_y += gy[h_offset + 1][w_offset + 1] * image[h + h_offset][w + w_offset].rgbtRed;
                        g_y += gy[h_offset + 1][w_offset + 1] * image[h + h_offset][w + w_offset].rgbtGreen;
                        b_y += gy[h_offset + 1][w_offset + 1] * image[h + h_offset][w + w_offset].rgbtBlue;
                    }
                }
            }
            // Compute g
            gr = round(sqrt(pow(r_x, 2) + pow(r_y, 2)));
            gg = round(sqrt(pow(g_x, 2) + pow(g_y, 2)));
            gb = round(sqrt(pow(b_x, 2) + pow(b_y, 2)));

            // if g > 255 reduce it to 255
            gr = (gr > 255) ? 255 : gr;
            gg = (gg > 255) ? 255 : gg;
            gb = (gb > 255) ? 255 : gb;


            image_copy[h][w].rgbtRed = gr;
            image_copy[h][w].rgbtGreen = gg;
            image_copy[h][w].rgbtBlue = gb;
        }
    }

    // copy the new image.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = image_copy[i][j];
        }
    }

}

int my_rounder(double real_value)
{
    double decimal_part = fmod(real_value, 1);

    if (decimal_part >= 0.5)
    {
        return ((int)real_value) + 1;
    }
    else
    {
        return (int)real_value;
    }
}

RGBTRIPLE average_rgb(int width, RGBTRIPLE neighbours[width])
{
    int red_counter = 0;
    int green_counter = 0;
    int blue_counter = 0;
    for (int i = 0; i < width; i++)
    {
        red_counter += neighbours[i].rgbtRed;
        green_counter += neighbours[i].rgbtGreen;
        blue_counter += neighbours[i].rgbtBlue;
    }
    RGBTRIPLE average;
    average.rgbtRed = my_rounder(red_counter / (double)width);
    average.rgbtGreen = my_rounder(green_counter / (double)width);
    average.rgbtBlue = my_rounder(blue_counter / (double)width);
    return average;
}