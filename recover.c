#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t BYTE;
BYTE buffer[512];

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // get card file name
    char *card = argv[1];

    //open card file
    FILE *cardptr = fopen(card, "r");
    if (cardptr == NULL)
    {
        fprintf(stderr, "Could not open %s\n", card);
        return 2;
    }

    // initialize found JPEG count
    int count = 0;

    // initialize file pointer
    FILE *img = NULL;

    // state
    bool in_progress = false;

    for (int i = 0; ; i++)
    {
        // read 512 bytes at a time / store in reading 1 / 0
        int reading = fread(&buffer, 512, 1, cardptr);

        // break loop at EOF
        if (!reading)
        {
            break;
        }

        // JPEG header found?
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // already found JPEG?
            if (in_progress)
            {
                fclose(img);
                count++;
            }

            // initialize file name
            char filename[7];
            sprintf(filename, "%03i.jpg", count);

            // starting new JPEG file
            in_progress = true;

            //open JPEG file
            img = fopen(filename, "w");

            // write to file
            fwrite(&buffer, sizeof(buffer), 1, img);

            // go to next block
            continue;
        }

        // already have a JPEG file open?
        if (in_progress)
        {
            // write to file
            fwrite(&buffer, sizeof(buffer), 1, img);
        }

    }

    //close JPEG file
    fclose(img);

    //close card file
    fclose(cardptr);

    // Success!
    return 0;
}
