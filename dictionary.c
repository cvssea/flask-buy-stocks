// Implements a dictionary's functionality
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "dictionary.h"

#define DICTIONARY "dictionaries/large"

// Represents number of buckets in a hash table
#define N 26

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[N];

// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
        // TODO
        // get hashed index
        int hashed = hash(word);

        // allocate memory for node
        node *new_node = malloc(sizeof(node));
        if (!new_node)
        {
            // out of memory
            unload();
            return false; // out of memory
        }

        // create node
        strcpy(new_node->word, word);
        new_node->next = NULL;

        // check if nothing at index
        if (!hashtable[hashed])
        {
            // nothing at index -> add new node
            hashtable[hashed] = new_node;
        }
        else
        {
            // add new node to beginning of list
            new_node->next = hashtable[hashed];
            hashtable[hashed] = new_node;
        }

        //free memory
        //free(new_node);
    }

    // Close dictionary
    fclose(file);

    // load test
    //printf("a word: %s\n", hashtable[0]->word);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    //open dictionary
    FILE *file = fopen(DICTIONARY, "r");
    if (!file)
    {
        printf("Could not open dictionary.\n");
        return 0;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // count words
    int size = 0;
    while (fscanf(file, "%s", word) != EOF)
    {
        size++;
    }
    fclose(file);
    return size;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    // get index
    int hashed = hash(word);

    //check for word
    for (node *ptr = hashtable[hashed]; ptr->next != NULL; ptr = ptr->next)
    {
        if (strcasecmp(ptr->word, word) == 0)
        {
            return true;
        }
    }

    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        node *cursor = hashtable[i];
        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    return true;
}
