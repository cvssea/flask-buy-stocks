from cs50 import get_int


def main():
    while True:
        height = get_int("Height: ")
        if height > 0 and height < 9:
            break
    mario(height)


def mario(height):
    gap = 3

    for i in range(height):
        for j in range(1, height + gap + i + 1):
            if j >= height - i:
                if j > height and j < height + gap:
                    print(" ", end="")
                else:
                    print("#", end="")
            else:
                print(" ", end="")
        print()


if __name__ == "__main__":
    main()