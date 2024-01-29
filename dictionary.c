// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table

// Making buckets according to the sum of the ascii values
const unsigned int N = 10000;

// Hash table
node *table[N];

// Count the number of words in dictionary
int word_counter = 0;

//---------------------------------------------------

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // hash word to obtain a hash value
    int index = hash(word);

    node *cursor = table[index]->next;

    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }

        cursor = cursor->next;
    }

    return false;
}

//---------------------------------------------------

// Hashes word to a number
unsigned int hash(const char *word)
{
    int hash = 7;
    // gets the sum of the ascii values of all characters in the word (excluding non-alphabetical characters)
    for (int i = 0; i < strlen(word); i++)
    {
        hash = hash * 33 + toupper(word[i]);
    }
    return hash % N;
}

//---------------------------------------------------

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Allocating memory to the table "buckets"
    for (int i = 0; i < N; i++)
    {
        table[i] = malloc(sizeof(node));
        if (table[i] == NULL)
        {
            return false;
        }
        table[i]->next = NULL;
    }

    // Open file
    FILE *file = fopen(dictionary, "r");

    // Check if file returns NULL
    if (file == NULL)
    {
        return false;
    }

    // Initialise variable to temporarily store word as it is read 1 by 1 from file
    char *word = malloc(sizeof(char) * (LENGTH + 1));

    // Read strings from File using fscanf
    while (fscanf(file, "%s", word) != EOF)
    {
        // Allocate memory for new node
        node *new_node = malloc(sizeof(node));

        // Check for NULL
        if (new_node == NULL)
        {
            return false;
        }
        new_node->next = NULL;

        // Copy word into node
        strcpy(new_node->word, word);

        // Hash function to determine which bucket it goes into
        int index = hash(word);

        // Set the next of new_node to be the first element of linked list
        new_node->next = table[index]->next;

        // Set head to be the new_node just created
        table[index]->next = new_node;

        word_counter++;
    }
    fclose(file);

    free(word);
    return true;
}

//---------------------------------------------------

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return word_counter;
}

//---------------------------------------------------

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // initial node variables
    node *cursor;
    node *temp;

    // loop through the table
    for (int i = 0; i < N; i++)
    {
        cursor = table[i];
        temp = cursor;

        // loop through each individual node of the linked list
        while (cursor != NULL)
        {
            cursor = cursor->next;
            free(temp);
            temp = cursor;
        }
    }
    return true;
}
