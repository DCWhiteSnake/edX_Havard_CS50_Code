// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include "dictionary.h"

char *to_upper(const char *str);
// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 91;

// Hash table
node *table[N];
unsigned int count = 0;
// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int word_id = (hash(word) % N);

    node *temp = table[word_id];
    int len = strlen(word);
    while (temp)
    {
        int temp_len = strlen(temp->word);
        if (temp_len == len && strncasecmp(word, temp->word, len) == 0)
        {
            return true;
        }
        temp = temp->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{

    char *u_w = to_upper(word);
    // TODO: Improve this hash function
    // DJB hash - http://partow.net/programming/hashfunctions/index.html
    int length = strlen(word);
    int hash = 5381;
    int i = 0;

    for (i = 0; i < length; ++i)
    {
        hash = ((hash << 5) + hash) + (*u_w);
    }
    free(u_w);
    return hash;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    count = 0;
    FILE *dict_txt = fopen(dictionary, "r");
    char word_buffer[LENGTH + 1];
    // char *word_buffer = malloc(sizeof(char) *[LENGTH + 1]);

    if (dict_txt == NULL)
    {
        return false;
    }

    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    while (fscanf(dict_txt, "%s", word_buffer) > 0)
    {
        count++;
        int id_hash = hash(word_buffer) % N;
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            fclose(dict_txt);
            return false;
        }
        strcpy(new_node->word, word_buffer);
        new_node->next = table[id_hash];
        table[id_hash] = new_node;
    }
    fclose(dict_txt);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    if (count == 0)
    {
        return false;
    }

    for (int i = 0; i < N; i++)
    {
        if (table[i] == 0)
        {
            continue;
        }
        node *temp = table[i];
        table[i] = table[i]->next;

        while (temp != NULL)
        {
            free(temp);
            if (table[i])
            {
                temp = table[i];
                table[i] = table[i]->next;
            }
            else
            {
                break;
            }
        }
    }
    return true;
}

char *to_upper(const char *str)
{
    int len = strlen(str);

    // allocate memory for result
    char *upper_str = (char *)malloc(len + 1);
    for (int i = 0; i < len; i++)
    {
        // convert current character to uppercase
        upper_str[i] = toupper(str[i]);
    }
    // add null terminator to end of string
    upper_str[len] = '\0';
    return upper_str;
}