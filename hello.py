from cs50 import get_string

def main():
    hello()

def hello():
    name = get_string("What is your name?\n")
    print(f"hello, {name}")

if __name__ == "__main__":
    main()