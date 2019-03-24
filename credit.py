from cs50 import get_int


def main():
    cc_number = get_cc()
    print(f"{card_type(cc_number)}")


def get_cc():
    cc_number = get_int("Number: ")
    return cc_number


def valid_luhn(cc_number):
    sum = 0
    while cc_number > 0:
        sum += cc_number % 10
        cc_number //= 10
        digit = (cc_number % 10) * 2
        if digit > 9:
            sum += 1 + (digit % 10)
        else:
            sum += digit
        cc_number //= 10
    return sum % 10 == 0


def card_type(cc_number):
    big_number = 1000000000000

    # check for valid card number
    if not valid_luhn(cc_number) and cc_number < big_number:
        return "INVALID"
    while cc_number > 100:
        cc_number //= 10
    # AMEX?
    if cc_number == 34 or cc_number == 37:
        return "AMEX"
    # MASTERCARD?
    elif cc_number > 50 and cc_number < 56:
        return "MASTERCARD"
    # VISA?
    elif cc_number // 10 == 4:
        return "VISA"
    # INVALID for any other cases
    else:
        return "INVALID"


if __name__ == "__main__":
    main()