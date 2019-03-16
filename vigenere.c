#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

bool is_alpha(string);
int shift(char);
string encrypt(string, string); 
char get_shifted_char(char, int);

int main(int argc, string argv[])
{
    if (argc == 2 && is_alpha(argv[1]))
    {
        string key = argv[1];
        string plain_text = get_string("plaintext: ");
        string cipher_text = encrypt(plain_text, key);
        printf("ciphertext: %s\n", cipher_text);
    }
    else
    {
        printf("Usage: ./vigenere keyword\n");
        return 1;
    }
}

bool is_alpha(string keyword)
{
    for (int i = 0, n = strlen(keyword); i < n; i++)
    {
        if (!isalpha(keyword[i]))
        {
            return false;
        }
    }
    return true;
}

int shift(char c)
{
    int shift_by;
    if (islower(c))
    {
        shift_by = c - 97;
    }
    else 
    {
        shift_by = c - 65;
    }
    return shift_by;
}

string encrypt(string plain_text, string key)
{
    string cipher_text = plain_text;
    int current_key = 0;
   
    for (int i = 0, n = strlen(plain_text); i < n; i++)
    {
        int c = (int)plain_text[i]; // left untouched if not letter
        if (isalpha(plain_text[i]))
        {
            c = get_shifted_char(c, shift(key[current_key]));
            current_key++;
            if (current_key == strlen(key))
            {
                current_key = 0;
            }
        }
        cipher_text[i] = c;
    } // end of for loop
    return cipher_text;
}

char get_shifted_char(char c, int key) 
{
    if (islower(c))
    {
        if (c + key > 122) 
        {
            c = (c + key) % 122 + 96; // wrapping
        }
        else 
        {
            c += key;   
        }
    }
    
    if (isupper(c))
    {
        if (c + key > 90) 
        {
            c = (c + key) % 90 + 64; //wrapping 
        }
        else
        {
            c = c + key;
        }
    }
    return c;
}
