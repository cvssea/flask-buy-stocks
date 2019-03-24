from cs50 import get_int


def main():
    while True:
        height = get_int("Height: ")
        if height > 0 and height < 9:
            break
    mario(height)


def mario(height):
    for i in range(height):
        for j in range(height):
            if j >= height - 1 - i:
                print("#", end="")
            else:
                print(" ", end="")
        print()


if __name__ == "__main__":
    main()

