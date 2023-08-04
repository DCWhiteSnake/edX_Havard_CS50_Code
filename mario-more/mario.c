#include <cs50.h>
#include <stdio.h>
#include <string.h>

void print_pyramid_row(int value, int max_pace)
{


    int count = 1;
    char left_partial[1000] = "";
    char right_partial[1000] = "";


    /// cook left
    // left should start [max paces] from the left.

    while (count <= max_pace)
    {
        strcat(left_partial, " ");
        count += 1;
    }
    // post-pend #
    // restart count
    count = 1;
    while (count <= value)
    {
        strcat(left_partial, "#");
        count += 1;

    }

    // restart count
    count = 1;

    /// cook right
    while (count <= value)
    {
        strcat(right_partial, "#");
        count += 1;
    }

    // join left and right
    string temp = left_partial;
    strcat(left_partial, "  ");
    strcat(left_partial, right_partial);

    // print
    printf("%s\n", left_partial);
}


void print_pyramid(int height)
{
    // currentbig counts the distance from the left edge, we decrement it
    // in the loop to create that step effect.
    int current_big = height - 1;
    for (int curr_height = 1; curr_height <= height; curr_height++)
    {
        print_pyramid_row(curr_height, current_big);
        current_big -= 1;
    }
}




int main(void)
{
    bool input_is_valid = false;
    int value = 0;
    while (!input_is_valid)
    {
        value = get_int("Input pyramid height: ");

        if (value > 0 && value < 9)
        {
            input_is_valid = true;
        }
    }

    print_pyramid(value);


}


