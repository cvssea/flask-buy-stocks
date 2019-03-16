#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

string encrypt(string, int);

int main(int argc, string argv[])
{
    if (argc == 2)
    {   
        for (int i = 0, n = strlen(argv[1]); i < n; i++)
        {
            if (!isdigit(argv[1][i]))
            {
                printf("Usage ./caesar key\n");
                return 1;
            }
        }
        int key = atoi(argv[1]);
        string plain_text = get_string("plaintext: ");
        string cipher_text = encrypt(plain_text, key);
        printf("ciphertext: %s\n", cipher_text);
    }
    else
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
}

string encrypt(string plain_text, int key)
{
    string cipher_text = plain_text;
    if (key > 25) 
    {
        key = key % 26; 
    }
    for (int i = 0, n = strlen(plain_text); i < n; i++)
    {
        int c = (int)plain_text[i]; // left untouched if not letter
        // is lowercase?
        if (c > 96 && c < 123)
        {
            if (c + key > 122) 
            {
                c = (c + key) % 122 + 96;
            }
            else 
            {
                c += key;   
            }
        }
        // is uppercase?
        if (c > 64 && c < 91)
        {
            if (c + key > 90) c = (c + key) % 90 + 64; //wrapping
            else c = c + key;
        }
        cipher_text[i] = c;
    } // end of for loop
    return cipher_text;
}
