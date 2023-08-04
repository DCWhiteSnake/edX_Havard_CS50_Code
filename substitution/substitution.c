#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

bool validate_key_elements_are_unique(string key);
bool validate_key_is_allalpha(string key);
string encrypt(string key, string text);
char find_mirror_index(string key, char character);

int main(int argc, string argv[])
{
    if (argc > 2 || argc == 0)
    {
        printf("Usage: ./substitution key\n");
    }
    else if (argc == 2 && !validate_key_is_allalpha(argv[1]))
    {
        printf("Usage: ./substitution key\n");
    }

    // if argument count is 2 but the key is less than 26 characters, inform the user that he needs a 26character unspaced key
    else if (argc == 2 && strlen(argv[1]) < 26)
    {
        printf("Key must contain 26 characters\n");
    }

    // if argument count is exactly two, and user put the correct amount of characters in key i.e., 26, and all the characters of the key are alphabets and finally that the elements
    // are unique the perform cypeher ops.
    else if (argc == 2 && strlen(argv[1]) == 26 && validate_key_is_allalpha(argv[1]) && validate_key_elements_are_unique(argv[1]))
    {
        string plain_text = get_string("plaintext:  ");
        string cipher_text = encrypt(argv[1], plain_text);

        printf("ciphertext: %s\n", cipher_text);
        //return zero to show successful program run
        return 0;
    }

    // and if the user is especially uncooperative just teach him how to use the app again.
    else
    {
        printf("Usage: ./substitution key\n");

    }
    return 1;
}

// encript the text using the key
string encrypt(string key, string text)
{

    // loop through all characters in "text" and if we find an alphabet charater then well cypher that character
    // in the original text i.e well leave non alphabetical input as is
    for (int i = 0; i < strlen(text); i++)
    {
        if (isalpha(text[i]))
        {
            text[i] = find_mirror_index(key, text[i]);
            //printf("text => %s\n", text);
        }
    }

    return text;
}

// Get the index of a single character in the users input and return it's mirror character in the
// english alphabet set;
char find_mirror_index(string key, char character)
{
    string alphabet_set = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < strlen(key); i++)
    {
        //printf("character == tolower(key[i]) => %d\n", tolower(key[i]) == tolower(character));
        if (tolower(alphabet_set[i]) == tolower(character))
        {
            if (islower(character))
            {
                return tolower(key[i]);
            }
            else
            {
                return toupper(key[i]);
            }

        }
    }
    return ' ';

}


// checks that all characters of the key are part of the english alphabet
bool validate_key_is_allalpha(string key)
{
    for (int i = 0; i < strlen(key); i++)
    {
        if (!isalpha(key[i]))
        {
            return false;
        }
    }
    return true;
}

// checks that the elements of the key are unique
bool validate_key_elements_are_unique(string key)
{
    // reduced to 2n.
    for (int i = 0; i < strlen(key); i++)
    {
        for (int j = i + 1; j < strlen(key) - 1; j++)
        {

            if (tolower(key[i]) == tolower(key[j]))
            {
                printf("badkey");
                return false;
            }
        }
    }
    return true;
}