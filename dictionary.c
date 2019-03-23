// Implements a dictionary's functionality
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "dictionary.h"

// Represents number of node pointers in array
#define N 27

// Represents a node in a trie
typedef struct node
{
    bool is_word;
    struct node *children[N];
}
node;

// head of trie
node *root;

// initialize dictionary word counter
unsigned int  WORD_COUNT = 0;

// creates a new node
struct node *get_node(void)
{
    // create and initialize to null
    node *ptr;
    ptr = calloc(1, sizeof(node));

    return ptr;
}

bool free_willy(node *ptr)
{
    for (int i = 0; i < N; i++)
    {
        if (ptr->children[i])
        {
            free_willy(ptr->children[i]);
        }
    }
    free(ptr);
    return true;
}

// Hashes word to a number between 0 and 25, inclusive, based on lsts first letter
unsigned int hash(const char letter)
{
    return tolower(letter) - 'a';
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize trie
    root = get_node();

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF)
    {
        int length = strlen(word);
        int index;
        node *ptr = root;

        for (int i = 0; word[i]; i++)
        {
            //check for apostrophe
            if (word[i] == '\'')
            {
                index = 26;
            }
            else
            {
                index = hash(word[i]);
            }

            //printf("index: %i\n", index);
            if (!ptr->children[index])
            {
                ptr->children[index] = get_node();
            }
            ptr = ptr->children[index];
        }

        // mark end of word
        ptr->is_word = true;
        WORD_COUNT++;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return WORD_COUNT;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int length = strlen(word);
    int index;
    node *ptr = root;

    // check every letter
    for (int i = 0; i < length; i++)
    {
        index = hash(word[i]);
        // if pointer is null word doesn't exist
        if (!ptr->children[index])
        {
            return false;
        }
        // pointer is set, go to next level
        ptr = ptr->children[index];
    }
    return (ptr != NULL && ptr->is_word);
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // the truth shall set you free
    return free_willy(root);
}