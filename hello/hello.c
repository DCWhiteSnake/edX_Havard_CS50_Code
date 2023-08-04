#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Ask user kindly for their name
    // The string class is not natural to C so cs50 module contains its
    // definition
    string name = get_string("What's you name? ");

    // Greet user
    printf("hello, %s\n", name);
}