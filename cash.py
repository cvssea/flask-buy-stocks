from cs50 import get_float


def main():
    change = int(change_owed())
    print(f"{coins_count(change)}")


def coins_count(change):
    # Initialize coins and count
    coins = [25, 10, 5, 1]
    count = 0

    # subtract highest possible coin from remaining change
    for coin in coins:
        while change >= coin:
            print(f"change left: {change}; coin used: {coin}")
            change -= coin
            count += 1
    return count


# Get input from user
def change_owed():
    while True:
        amount = get_float("Change owed: ")
        if amount > 0:
            break
    return amount * 100


if __name__ == "__main__":
    main()