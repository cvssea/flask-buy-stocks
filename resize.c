#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }

    // resize factor
    int n = atoi(argv[1]);
    printf("n: %i\n", n);

    // filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf, n_bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    fseek(inptr, -sizeof(BITMAPFILEHEADER), SEEK_CUR);
    fread(&n_bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi, n_bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    fseek(inptr, -sizeof(BITMAPINFOHEADER), SEEK_CUR);
    fread(&n_bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // set new width / height
    n_bi.biWidth = bi.biWidth * n;
    n_bi.biHeight = bi.biHeight * n;

    // determine new padding
    int n_padding = (4 - (n_bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // new sizes
    n_bi.biSizeImage = ((sizeof(RGBTRIPLE) * n_bi.biWidth) + n_padding) * abs(n_bi.biHeight);
    n_bf.bfSize = n_bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write new BITMAPFILEHEADER to outfile
    fwrite(&n_bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write new BITMAPINFOHEADER to outfile
    fwrite(&n_bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine infile padding
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    //iterate over scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // array to store new row
        RGBTRIPLE new_row[n_bi.biWidth];

        // array position count
        int count = 0;

        //iterate over pixels in scanline
        for (int j = 0, biWidth = bi.biWidth; j < biWidth; j++)
        {
            RGBTRIPLE triple;
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // write to array n times
            for (int k = 0; k < n; k++)
            {
                new_row[count] = triple;
                count++;
            }
        }

        // write array to outfile n times and add padding if necessary
        for (int l = 0; l < n; l++)
        {
            fwrite(&new_row, sizeof(new_row), 1, outptr);
            if (n_padding)
            {
                for (int m = 0; m < n_padding; m++)
                {
                    fputc(0x00, outptr);
                }
            }
        }

        //skip over infile padding if any
        fseek(inptr, padding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}