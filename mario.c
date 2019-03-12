#include <cs50.h>
#include <stdio.h>

int get_height(void);
void build_pyramid(int, int);

int main(void)
{
    int height = get_height();
    build_pyramid(height, 2);
}

int get_height()
{
    int height = get_int("Enter height between 1 - 8: ");
    while (height < 1 || height > 8) 
    {
        height = get_height();
    }
    return height;
}

void build_pyramid(int height, int gap) 
{
    int width = height * 2 + gap;
    gap += 1;
    for (int i = 0; i < height; i++) 
    {
        for (int j = 1; j <= height + gap + i; j++)
        {
            if (j >= height - i)
            {
                if (j > height && j < height + gap)
                {
                    printf(" ");
                }
                else 
                {
                    printf("#");
                }
            }
            else
            {
                printf(" ");   
            }
        }
        printf("\n");
    }
}
