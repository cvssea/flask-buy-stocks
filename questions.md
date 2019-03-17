# Questions

## What's `stdint.h`?

Allows programmers to write more portable code by setting the size of integer types, thus the code can run correctly on other machines.

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

Specifies the exact size of int you need.

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

- `BYTE` is is 1 byte (8 bits unsigned)
- `DWORD` has 4 bytes (32bits unsigned) 
- `LONG` has 4 bytes (32bits signed)
- `WORD` has 2 bytes (16bits unsigned)

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

 BM is ASCII or 0x42 0x4D in hexadecimal 

## What's the difference between `bfSize` and `biSize`?

- `bfSize` is the size of the bitmap file including headers
- `biSize` size of the `BITMAPINFOHEADER` structure

## What does it mean if `biHeight` is negative?

It means that the bitmap is a top-down DIB with it's origin at the upper left corner. 

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

* `biBitCount`

## Why might `fopen` return `NULL` in `copy.c`?

The file might be missing

## Why is the third argument to `fread` always `1` in our code?

It's only reading one integer from the file

## What value does `copy.c` assign to `padding` if `bi.biWidth` is `3`?

TODO

## What does `fseek` do?

TODO

## What is `SEEK_CUR`?

TODO
