from sys import argv, exit
from cs50 import get_string


def main():
    # usage
    while True:
        if len(argv) == 2:
            key = int(argv[1])
            break
        else:
            exit("Usage: python caesar.py key")
    # user input
    plain_text = get_string("plaintext: ")
    print(f"ciphertext: {encrypt(plain_text, key)}")


# returns encrypted text based on numeric key
def encrypt(plain_text, key):
    cipher_text = ""
    # truncate key
    if key > 25:
        key %= 26

    # encrypt char by char
    for c in plain_text:
        # continue if not letter
        if not c.isalpha():
            cipher_text += c
            continue
        # cast to int
        e = ord(c)
        # uppercase
        if c.isupper():
            if (e + key) > 90:
                # wrap
                c = chr((e + key) % 90 + 64)
            else:
                c = chr(e + key)
        # lowercase
        elif (e + key) > 122:
            # wrap
            c = chr((e + key) % 122 + 96)
        else:
            c = chr(e + key)
        # add letter to text
        cipher_text += c
    return cipher_text


if __name__ == "__main__":
    main()