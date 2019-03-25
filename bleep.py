from cs50 import get_string
from sys import argv


def main():
    # enforce correct usage
    if not len(argv) == 2:
        exit("Usage: python bleep.py dictionary")

    # create dictionary
    dictionary = create_dictionary(argv[1])

    # user input
    uncensored = get_string("What message would you like to censor?\n")

    # output
    print(censor(dictionary, uncensored))


# creates a set from a given file
def create_dictionary(f):
    # open file
    file = open(f, 'r')
    if not file:
        exit("Could not open file")

    # create dictionary from file
    dictionary = set()
    for word in file:
        dictionary.add(str.strip(word))

    return dictionary


# returns censored phrase based on dictionary
def censor(dictionary, uncensored):
    # censorship is bad
    uncensored = uncensored.split()

    for word in uncensored:
        # lowercase inputted words and compare to dictionary
        f_word = word.lower()
        if f_word in dictionary:
            index = uncensored.index(word)
            uncensored[index] = '*' * len(word)

    censored = ' '.join(uncensored)

    return censored


if __name__ == "__main__":
    main()
