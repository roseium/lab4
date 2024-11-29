#include <stdio.h>
#include <stdlib.h>

int is_valid_float(const char *str);

int main()
{
    FILE *inputFile, *outputFile, *processedFile;
    char value[50];
    int invalidCount = 0;

    // Open input file for read
    inputFile = fopen("data.txt", "r");

    // Open output file for valid floats
    outputFile = fopen("valid_floats.txt", "w");

    // Open a new file to store the processed input
    processedFile = fopen("processed_data.txt", "w");

    // Process file content
    while (fscanf(inputFile, "%49s", value) != EOF)
    {
        fprintf(processedFile, "%s\n", value);

        if (is_valid_float(value))
        {
            fprintf(outputFile, "%s\n", value);
        }

        else
        {
            invalidCount++;
        }
    }

    printf("Number of invalid values: %d\n", invalidCount);

    fclose(inputFile);
    fclose(outputFile);
    fclose(processedFile);

    return 0;
}

int is_valid_float(const char *str)
{
    char *endp;
    if (*str == '\0')
    {
        return 0;
    }

    strtod(str, &endp);
    return (*endp == '\0' && endp != str);
}
