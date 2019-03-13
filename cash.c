#include <math.h>
#include <cs50.h>
#include <stdio.h>

int change_owed(void);
int coins_count(int);

int main(void) 
{   
    int change = change_owed();
    printf("%i\n", coins_count(change));
}

int coins_count(int change)
{
    int coins[] = {25, 10, 5, 1};
    int count = 0;
    
    for (int i = 0; i < 4; i++)
    {
        while (change >= coins[i])
        {
            change -= coins[i];
            count++;
        }
    }
    return count;
}

int change_owed()
{
    float amount;
    do
    {
        amount = get_float("Change owed: ");       
    }
    while (amount <= 0);
    return (int) round(amount * 100);
}
