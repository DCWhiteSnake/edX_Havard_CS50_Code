#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

// header
int calculate_coleman_liau_index(string sentence);

int main(void)
{
    string sentence = get_string("Text: ");

    int index = calculate_coleman_liau_index(sentence);

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 1 && index <= 16)
    {
        printf("Grade %i\n", index);
    }
    else
    {
        printf("Grade 16+\n");
    }
}

// counts the letters in text
int count_letters(string text)
{
    int letter_count = 0;
    int sentence_length = strlen(text);

    if (sentence_length > 0)
    {
        for (int i = 0; i < sentence_length; i++)
        {
            if (isalpha(text[i]))
            {
                letter_count = letter_count + 1;
            }
        }
    }
    return letter_count;
}

// counts the number of words in text
int count_words(string text)
{
    int word_count = 0;
    int sentence_length = strlen(text);

    if (sentence_length > 0)
    {
        for (int i = 0; i < sentence_length; i++)
        {
            if (i == 0)
            {
                word_count = word_count + 1;
            }
            if (text[i] == 32)
            {
                word_count = word_count + 1;
            }

        }

    }

    return word_count;

}

// counts the amount of sentences in the input aka text.
int count_sentences(string text)
{
    int sentence_count = 0;
    int sentence_length = strlen(text);

    if (sentence_length > 0)
    {
        for (int i = 0; i < sentence_length; i++)
        {
            // If we encounter a ".', "?" or an "!" then that indicates a sentence ending
            if (text[i] == 46 || text[i] == 33 || text[i] == 63)
            {
                sentence_count = sentence_count + 1;
            }
        }
    }
    return sentence_count;
}

/// @brief  Calculates the Coleman-Liau index.
/// @param sentence The sentence to calculate the Coleman-Liau index for.
/// @return the Coleman-Liau index
int calculate_coleman_liau_index(string sentence)
{
    // get the required variables to calculate the index
    int letter_count = count_letters(sentence);
    double word_count = count_words(sentence);
    double sentence_count = count_sentences(sentence);

    // calculate the letter count  and sentence count per 100 words
    double l = (letter_count / word_count) * 100;
    double s = (sentence_count / word_count) * 100;

    // finally calculate the Coleman-Liau index
    double index = 0.0588 * l - 0.296 * s - 15.8;

    // return the integer representation of the index using the round method
    return round(index);
}




