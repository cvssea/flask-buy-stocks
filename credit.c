#include <cs50.h>
#include <stdio.h>

long get_cc(void);
int is_valid_luhn(long);
string card_type(long);

int main(void)
{
    long cc_number = get_cc();
    printf("%s", card_type(cc_number));
}

long get_cc()
{
    long cc_number = get_long("Number: \n");
    return cc_number;
}

int is_valid_luhn(long cc_number)
{
    int sum = 0;
    
    while (cc_number > 0)
    {
        sum += cc_number % 10;
        cc_number /= 10;
        int digit = (cc_number % 10) * 2;
        digit > 9 ? (sum += 1 + (digit % 10)) : (sum += digit);
        cc_number /= 10;
    }
    
    return sum % 10 == 0;
}

string card_type(long cc_number)
{
    if (!is_valid_luhn(cc_number) || cc_number < 1000000000000)
    {
        return "INVALID\n";
    }
    while (cc_number > 100)
    {
        cc_number /= 10;
    }
    if (cc_number == 34 || cc_number == 37) 
    {
        return "AMEX\n";
    } 
    else if (cc_number == 51 || cc_number == 52 || cc_number == 53 || cc_number == 54 || cc_number == 55)
    {
        return "MASTERCARD\n";
    }
    else if (cc_number / 10 == 4)
    {
        return "VISA\n";
    }
    else 
    {
        return "INVALID\n";
    }
}
