// Sequential bubblesort c program

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

void bubblesort(float* array, int arraySize);

int main(int argc, char **argv) {
    char *csvcheck = ".csv";
    char *inputCSV;
    char *outputCSV;
    float *flatarr;
    int arraySize;

    clock_t start, end;
    double cpu_time_used;
    printf("**************************Running Sequential Bubblesort program**************************\n");

    if (argc > 1) {
        if (strstr(argv[argc-1], csvcheck)) {
            outputCSV = argv[argc-1];
        }
        if (strstr(argv[argc - 2], csvcheck)) {
            inputCSV = argv[argc - 2];
        }
    }

    sscanf(inputCSV, "%ix%i", &arraySize, &arraySize);

    start = clock();
        FILE *filein = fopen(inputCSV, "r");
        if (filein == NULL) {
            printf("Could not open file\n");
            return 1;
        }

        // Creates an array and stores the random float values from the inputCSV file in that array
        flatarr = malloc(arraySize*arraySize * sizeof(float)); // Flattened array instead of 2D, simpler to split
        for (int i = 0; i < arraySize*arraySize; i++) {
            if (fscanf(filein, "%f,", &flatarr[i]) != 1) {
                printf("Failed to read a float value.\n");
                return 1;
            }
        }
        // printf("Read float values successfully\n");
        end = clock();

        // Closes file
        fclose(filein);

        cpu_time_used = ((double)(end - start))/CLOCKS_PER_SEC;
        printf("Time to load input for %s: %fms\n", inputCSV, cpu_time_used*1000);

        start = clock();
        bubblesort(flatarr, arraySize);
        end = clock();

        cpu_time_used = ((double)(end - start))/CLOCKS_PER_SEC;
        printf("Processing time for %s: %fms\n", inputCSV, cpu_time_used*1000);

        // Uncomment lines below to enable printing of output arrays to the terminal
        // for (int n = 0; n < arraySize; n++) {
        //     printf("%f ", flatarr[n]);
        // }
        // printf("\n");

        start = clock();
        // Print out array to csv file
        // printf("Printing to CSV file...\n");
        FILE *csvFile = fopen(outputCSV, "w");
        if (csvFile == NULL) {
            printf("Could not open file\n");
            return 1;
        }
        
        for (int i = 0; i < arraySize; i++) {
            float val = flatarr[i];
            fprintf(csvFile, "%f,", val);
        }

        end = clock();
        fclose(csvFile);

        cpu_time_used = ((double)(end - start))/CLOCKS_PER_SEC;
        printf("Time to save result to output file %s for %s: %fms\n", outputCSV, inputCSV, cpu_time_used*1000);
}

void bubblesort(float* array, int arraySize) {
    // Var that tracks if the array is sorted or not
    bool sorted = false;

    while (!sorted) {
        // Assumes that the array is sorted initially
        sorted = true;

        for (int i = 0; i < arraySize - 1; i++) {
            if (array[i] > array[i+1]) {
                // Swaps elements at positions [i] and [i+1]
                float temp = array[i];
                array[i] = array[i+1];
                array[i+1] = temp;

                // If anything was not sorted, sorted is set to false and the while loop iterates through again
                sorted = false;
            }
        }
    }
}