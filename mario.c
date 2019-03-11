#include <cs50.h>
#include <stdio.h>

void mario(int);

int main(void)
{    
    int height = get_int("Enter height: ");
    while (height < 1 || height > 8) {
        height = get_int("Enter height: ");
    }
    mario(height);  
}

void mario(int height) {
    for (int i = 1; i <= height; i++) {
        for (int j = height; j > 0; j--) {
            j <= i ? printf("#") : printf(" ");
        }
        printf("\n");
    }
}
