#include <cs50.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

unsigned int hash(const char letter)
{
    return tolower(letter) - 'a';
}

typedef struct node
{
    bool is_word;
    struct node *children[26];
}
node;

node *root;

int main(void)
{
    char *word = get_string("Enter word: ");

    // initialize root
    root = calloc(1, sizeof(node));

    node *ptr = root;

    int level = 0;

    // insert word in trie
    for (int i = 0; word[i]; i++)
    {
        printf("letter: %c\n", word[i]);
        int hashed = hash(word[i]);

        if (!ptr->children[hashed])
        {
            ptr->children[hashed] = calloc(1, sizeof(node));
            ptr = ptr->children[hashed];
            level++;
            printf("added children[%i] on level %i\n", hashed, level);
        }

        if (i == strlen(word) - 1)
        {
            ptr->is_word = true;
            printf("inserted word!\n");
        }
    }

    node *trav = root;

    for (int i = 0; i < 26; i++)
    {
        if (trav->children[i])
        {
            printf("read letter at position %i\n", i);

        }
        trav = trav->children[i];
        if (trav->is_word)
        {
            printf("reached end of word");
        }
    }

}