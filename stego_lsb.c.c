#include <stdio.h>
#include <stdlib.h>

#define WIDTH 512
#define HEIGHT 512

// Function to read a PGM image in text format (P2)
int readPGMText(const char *filename, unsigned char *pixels, int width, int height)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening file");
        return -1;
    }

    char buffer[16];
    fgets(buffer, sizeof(buffer), file); // Read the PGM format identifier (e.g., "P2")

    // Check if the file is a valid P2 PGM file
    if (buffer[0] != 'P' || buffer[1] != '2')
    {
        fprintf(stderr, "Invalid PGM format. Expected P2.\n");
        fclose(file);
        return -1;
    }


    do
    {
        fgets(buffer, sizeof(buffer), file);
    } while (buffer[0] == '#');

    int imgWidth, imgHeight, maxGray;
    sscanf(buffer, "%d %d", &imgWidth, &imgHeight);


    // Validate image dimensions
    if (imgWidth != width || imgHeight != height)
    {
        fprintf(stderr, "Image dimensions do not match. Expected %dx%d.\n", width, height);
        fclose(file);
        return -1;
    }

    fscanf(file, "%d", &maxGray); // Read the maximum gray value
    if (maxGray != 255) {
        fprintf(stderr, "Unsupported max gray value. Expected 255.\n");
        fclose(file);
        return -1;
    }

    // Read pixel values
    for (int i = 0; i < width * height; ++i)
    {
        fscanf(file, "%hhu", &pixels[i]);
    }

    fclose(file);
    return 0;
}

// Function to write a PGM image in text format (P2)
int writePGMText(const char *filename, unsigned char *pixels, int width, int height)
{
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    // Write the PGM header
    fprintf(file, "P2\n%d %d\n255\n", width, height);

    // Write pixel values
    for (int i = 0; i < width * height; ++i)
    {
        fprintf(file, "%hhu\n", pixels[i]);
    }

    fclose(file);
    return 0;
}

// Function to write a PGM image in binary format (P5)
int writePGMBinary(const char *filename, unsigned char *pixels, int width, int height)
{
    FILE *file = fopen(filename, "wb");
    if (!file)
    {
        perror("Error opening file");
        return -1;
    }

    // Write the PGM header
    fprintf(file, "P5\n%d %d\n255\n", width, height);

    // Write pixel data as raw bytes
    fwrite(pixels, sizeof(unsigned char), width * height, file);

    fclose(file);
    return 0;
}

// Function to embed the secret image into the cover image using 4-bit LSB
void embedLSB(unsigned char *coverPixels, unsigned char *secretPixels, int width, int height)
{
    for (int i = 0; i < width * height; ++i)
    {
        coverPixels[i] = (coverPixels[i] & 0xF0) | (secretPixels[i] >> 4);
    }
}

// Function to extract the secret image from the stego image
void extractLSB(unsigned char *stegoPixels, unsigned char *outputPixels, int width, int height)
{
    for (int i = 0; i < width * height; ++i)
    {
        outputPixels[i] = (stegoPixels[i] & 0x0F) << 4;
    }
}

int main()
{
    // File names
    const char *coverImageFile = "baboon.pgm";
    const char *secretImageFile = "farm.pgm";
    const char *stegoImageFile = "stego_image_bin.pgm";
    const char *extractedSecretFile = "extracted_secret.pgm";

    // Allocate memory for images
    unsigned char *coverPixels = (unsigned char *)malloc(WIDTH * HEIGHT);
    unsigned char *secretPixels = (unsigned char *)malloc(WIDTH * HEIGHT);
    unsigned char *outputPixels = (unsigned char *)malloc(WIDTH * HEIGHT);

    if (!coverPixels || !secretPixels || !outputPixels) {
        fprintf(stderr, "Memory allocation failed.\n");
        return -1;
    }

    // Load the cover image
    if (readPGMText(coverImageFile, coverPixels, WIDTH, HEIGHT) != 0)
    {
        free(coverPixels);
        free(secretPixels);
        free(outputPixels);
        return -1;
    }

    // Load the secret image
    if (readPGMText(secretImageFile, secretPixels, WIDTH, HEIGHT) != 0)
    {
        free(coverPixels);
        free(secretPixels);
        free(outputPixels);
        return -1;
    }

    // Embed the secret image into the cover image
    embedLSB(coverPixels, secretPixels, WIDTH, HEIGHT);

    // Write the stego image in binary format
    if (writePGMBinary(stegoImageFile, coverPixels, WIDTH, HEIGHT) != 0)
    {
        free(coverPixels);
        free(secretPixels);
        free(outputPixels);
        return -1;
    }

    // Extract the secret image from the stego image
    extractLSB(coverPixels, outputPixels, WIDTH, HEIGHT);

    // Write the extracted secret image in text format
    if (writePGMText(extractedSecretFile, outputPixels, WIDTH, HEIGHT) != 0)
    {
        free(coverPixels);
        free(secretPixels);
        free(outputPixels);
        return -1;
    }

    // Free memory
    free(coverPixels);
    free(secretPixels);
    free(outputPixels);

    printf("Steganography process completed successfully.\n");
    return 0;
}
