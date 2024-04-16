// Simple program to generate csv files containing arrays of random numbers
#include <stdio.h>
#include <stdlib.h>
float** generateArray(int arrSize);
void printToCSV(float** arr, int arrSize);
void freeArrMem(float** arr, int arrSize);

int main (int argc, char **argv) {
    int arrSize = 0;

    for (int i = 1; i < argc; i++) {
        arrSize = atoi(argv[i]);
        float** array = generateArray(arrSize);
        printToCSV(array, arrSize);
        freeArrMem(array, arrSize);
    }
    return 0;
}

// Function to generate a square array of random float numbers of size arrSizexarrSize
float** generateArray(int arrSize) {
    printf("Making array\n");
    float** array = malloc(arrSize * sizeof(float*));
    for (int row = 0; row < arrSize; row++) {
        array[row] = malloc(arrSize * sizeof(float));
        for (int col = 0; col < arrSize; col++) {
            array[row][col] = (float)rand()/(float)(RAND_MAX);
        }
    }
    printf("%ix%i Array successfully generated!\n", arrSize, arrSize);
    return array;
}

// Prints an array with size arrSize to a csv file with name arrSizexarrSize.csv
void printToCSV(float** arr, int arrSize) {
    printf("Printing to CSV file...\n");
    char filename[110];
    sprintf(filename, "%ix%i.csv", arrSize, arrSize);
    FILE *csvFile = fopen(filename, "w");
    if (csvFile == NULL) {
        printf("Could not open file\n");
        return;
    }
    
    for (int row = 0; row < arrSize; row++) {
        for (int col = 0; col < arrSize; col++) {
            float val = arr[row][col];
            fprintf(csvFile, "%f,", val);
        }
    }

    fclose(csvFile);
}

// Frees up the memory for the array that was dynamically allocated onto the heap 
void freeArrMem(float** arr, int arrSize) {
    if (arr != NULL) {
        for (int i = 0; i < arrSize; i++) {
            if (arr[i] != NULL) free(arr[i]);
        }
        free(arr);
    }
}