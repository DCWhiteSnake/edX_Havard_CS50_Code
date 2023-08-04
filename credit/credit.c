#include <cs50.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

// check that the user input is unsigned
bool input_is_unsigned(long userInput)
{
    if (userInput > 0)
    {
        return true;
    }
    return false;
}

// Gets the length of the user input
int get_size(long userInput)
{
    if (input_is_unsigned(userInput))
    {
        char string_rep[100];
        sprintf(string_rep, "%ld", userInput);
        return strlen(string_rep);
    }
    return 0;
}


// Check that the card is infact a valid card
bool checksum(int input[], int inputSize)
{

    // x2Sum will be used to aggregate the digits multiplied by 2
    int x2Sum = 0;

    int secondToLastIndex = inputSize - 2;
    for (int i = secondToLastIndex; i >= 0; i -= 2)
    {
        // if the value of index i is >= 10 split it into 2 and then add otherwise add it as is to x2Sum
        if (input[i] * 2 < 10)
        {
            x2Sum += input[i] * 2;
        }
        else
        {
            int rem = (input[i] * 2) % 10;
            int quo = (input[i] * 2) / 10;
            x2Sum += rem + quo;
        }
    }


    int sum = 0;
    for (int i = inputSize - 1; i >= 0; i -= 2)
    {
        sum += input[i];
    }

    int total = x2Sum + sum;

    // if total is a multiple of 10
    if (total % 10 == 0)
    {
        return true;
    }
    return false;
}

/// Check if the card is an American Exchange card
bool IsAE(int input[], int inputSize)
{
    if (inputSize != 15)
    {
        return false;
    }
    // Yes at this point input is valid so you can check at aindex 0 and 1
    if ((input[0] == 3 && input[1] == 4) || (input[0] == 3 && input[1] == 7))
    {
        return true;
    }
    return false;
}

/// Check if the card is a VisaCard
bool IsVC(int input[], int inputSize)
{
    if (inputSize != 16 && inputSize != 13)
    {
        return false;
    }

    if (input[0] == 4)
    {
        return true;
    }
    return false;
}

// Check if the card is a MasterCard
bool IsMC(int input[], int inputSize)
{
    if (inputSize != 16)
    {
        return false;
    }

    if (
        (input[0] == 5 && input[1] == 1) ||
        (input[0] == 5 && input[1] == 2) ||
        (input[0] == 5 && input[1] == 3) ||
        (input[0] == 5 && input[1] == 4) ||
        (input[0] == 5 && input[1] == 5))
    {
        return true;
    }
    return false;
}

int main(void)
{

    long input = get_long("Input your card number: ");
    int count = get_size(input);

    if (count < 13 || count > 16 || count == 14)
    {
        printf("INVALID\n");
        return 0;
    }
    //
    int split_input[count];

    // get a power of 10 which we will use to split the input into integer singletons.
    // i.e a number with a size of 13 needs 10^12 to get the 1st number.
    long count_2 = pow(10, count - 1);

    // tracks the index to store the parsed number in the split_input array.
    int index = 0;

    // now put these singeltons in an array
    while (count_2 >= 1)
    {
        long temp = input / count_2;
        long rem = input % count_2;

        split_input[index] = temp;
        input = rem;
        index += 1;

        // reduce count_2 by a factor of /10
        count_2 = count_2 / 10;
    }

    // if the card is at least a valid card then procede to checking if it is one of them after which you print out the corresponding name.
    if (checksum(split_input, count))
    {
        if (IsAE(split_input, count))
        {
            printf("AMEX\n");
        }

        else if (IsMC(split_input, count))
        {
            printf("MASTERCARD\n");
        }
        else if (IsVC(split_input, count))
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }

    else
    {
        printf("INVALID\n");
    }
    return 0;
}