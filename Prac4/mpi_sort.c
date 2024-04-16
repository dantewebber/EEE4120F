/*
  Bubblesort MPI Parallelisation Program
*/
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

// Function declarations
void bubblesort(float* arrChunk, int chunkSize);
void merge(float* arr1, float* arr2, int size1, int size2, float* mergedArr);
void freeArrMem(float* arr, int arrSize);

int main(int argc, char **argv)
{
    char buf[256];
    int my_rank, num_procs, arrSize, chunkSize, chunkSize4, sortedArrSize;
    float *arrChunk;
    float *arrChunk4;
    float *arr1;
    float *arr2;
    float *mergedArr1;
    float *mergedArr2;
    float *flatarr;
    float *arrChunk2;
    float *mergedArr;
    float *sortedArr;

    char *csvcheck = ".csv";
    char *inputCSV;
    char *outputCSV;

    clock_t start, end;
    double cpu_time_used;

    if (argc > 1) {
        if (strstr(argv[argc-1], csvcheck)) {
            outputCSV = argv[argc-1];
        }
        if (strstr(argv[argc - 2], csvcheck)) {
            inputCSV = argv[argc - 2];
        }
    }

    start = clock();
    /* Initialize the infrastructure necessary for communication */
    MPI_Init(&argc, &argv);

    /* Identify this process */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    /* Find out how many total processes are active */
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    if (my_rank==0) printf("**************************Running MPI Parallelised Bubblesort program**************************\n");

    end = clock();
    cpu_time_used = ((double)(end - start))/CLOCKS_PER_SEC;
    printf("Time to start MPI for %s (rank: %i): %fms\n", inputCSV, my_rank, cpu_time_used*1000);

    /* Until this point, all programs have been doing exactly the same.
       Here, we check the rank to distinguish the roles of the programs */
    if (my_rank == 0) {
        // printf("Output csv: %s, Input csv: %s\n", outputCSV, inputCSV);

        sscanf(inputCSV, "%ix%i", &arrSize, &arrSize);
        chunkSize = (arrSize*arrSize)/num_procs;
        // printf("Chunk size: %i\n", chunkSize);
        int rem = (arrSize*arrSize)%num_procs;
        // printf("Remainder value: %i\n", rem);
        chunkSize4 = chunkSize + rem;
        arrChunk = malloc(chunkSize * sizeof(float));
        arrChunk4 = malloc(chunkSize4 * sizeof(float));

        start = clock();
        FILE *filein = fopen(inputCSV, "r");
        if (filein == NULL) {
            printf("Could not open file\n");
            return 1;
        }

        // Creates an array and stores the random float values from the inputCSV file in that array
        flatarr = malloc(arrSize*arrSize * sizeof(float)); // Flattened array instead of 2D, simpler to split
        for (int i = 0; i < arrSize*arrSize; i++) {
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

        int other_rank;
        // printf("We have %i processes.\n", num_procs);

        start = clock();
        /* Send messages to all other processes */
        for (other_rank = 1; other_rank < num_procs; other_rank++)
        {
            // printf("Sending data to process %i\n", other_rank);
            if (other_rank == (num_procs-1)) {
                // printf("Chunk 4 size: %i\n", chunkSize4);
                MPI_Send(&chunkSize4, 1, MPI_INT, other_rank, 0, MPI_COMM_WORLD);
                memcpy(arrChunk4, &flatarr[chunkSize*other_rank], chunkSize4*sizeof(float)); // copies chunkSize elements from flatarr into the arrchunks to be passed to each process
                MPI_Ssend(arrChunk4, chunkSize4, MPI_FLOAT, other_rank, 0, MPI_COMM_WORLD); // Sends the array chunks to each of the processes
                // freeArrMem(arrChunk4, chunkSize4);
            } else {
                MPI_Send(&chunkSize, 1, MPI_INT, other_rank, 0, MPI_COMM_WORLD);
                memcpy(arrChunk, &flatarr[chunkSize*other_rank], chunkSize*sizeof(float)); // copies chunkSize elements from flatarr into the arrchunks to be passed to each process
                MPI_Ssend(arrChunk, chunkSize, MPI_FLOAT, other_rank, 0, MPI_COMM_WORLD); // Sends the array chunks to each of the processes
                // freeArrMem(arrChunk, chunkSize);
            }
        }
        // printf("Bubble sorting partition 1...\n");
        // Bubblesort first partition of array
        arr1 = malloc(chunkSize * sizeof(float));
        memcpy(arr1, flatarr, chunkSize * sizeof(float));
        bubblesort(arr1, chunkSize);
        // for (int n = 0; n < chunkSize; n++) {
        //     printf("%f ", arr1[n]);
        // }
        // printf("\n");

        // printf("Receiving array from 2nd thread...\n");
        // Receive array from second thread
        int chunk2size = 0;
        MPI_Recv(&chunk2size, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // printf("chunk2size: %i\n", chunk2size);
        arr2 = malloc(chunk2size * sizeof(float));
        MPI_Recv(arr2, chunk2size, MPI_FLOAT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // for (int n = 0; n < chunk2size; n++) {
        //     printf("%f ", arr2[n]);
        // }
        // printf("\n");

        // printf("Merging partitions 1 and 2...\n");
        // Merge First 2 partitions of thread 
        int mergedArr1Size = chunkSize + chunk2size;
        // printf("mergedArr1Size: %i\n", mergedArr1Size);
        mergedArr1 = malloc(mergedArr1Size * sizeof(float));
        merge(arr1, arr2, chunkSize, chunk2size, mergedArr1);

        // for (int n = 0; n < mergedArr1Size; n++) {
        //     printf("%f ", mergedArr1[n]);
        // }
        // printf("\n");
        
        // printf("Receiving merged array from 3rd thread...\n");
        // Receive merged array from thread 3
        int mergedArr2Size;
        MPI_Recv(&mergedArr2Size, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // printf("mergedArr2Size: %i\n", mergedArr2Size);
        mergedArr2 = malloc(mergedArr2Size * sizeof(float));
        MPI_Recv(mergedArr2, mergedArr2Size, MPI_FLOAT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // for (int n = 0; n < mergedArr2Size; n++) {
        //     printf("%f ", mergedArr2[n]);
        // }
        // printf("\n");

        // printf("Merging into final array...\n");
        // Merge arrays into final sorted array
        sortedArrSize = (mergedArr1Size + mergedArr2Size);
        sortedArr = malloc (sortedArrSize * sizeof(float));
        merge(mergedArr1, mergedArr2, mergedArr1Size, mergedArr2Size, sortedArr);

        end = clock();

        cpu_time_used = ((double)(end - start))/CLOCKS_PER_SEC;
        printf("Processing time for %s: %fms\n", inputCSV, cpu_time_used*1000);

        // for (int n = 0; n < sortedArrSize; n++) {
        //     printf("%f ", sortedArr[n]);
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
        
        for (int i = 0; i < sortedArrSize; i++) {
            float val = sortedArr[i];
            fprintf(csvFile, "%f,", val);
        }

        end = clock();
        fclose(csvFile);

        cpu_time_used = ((double)(end - start))/CLOCKS_PER_SEC;
        printf("Time to save result to output file %s for %s: %fms\n", outputCSV, inputCSV, cpu_time_used*1000);

    } else {
        // Recieves the size of the array chunk to be sorted
        MPI_Recv(&chunkSize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        arrChunk = malloc(chunkSize * sizeof(float));
        // Receives the array chunk to be sorted
        MPI_Recv(arrChunk, chunkSize, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // printf("Process %i received chunk of size: %i\n", my_rank, chunkSize);

        bubblesort(arrChunk, chunkSize);

        // If thread 2 or 4, send data to thread 1 or 3 to be merged, and then 3 send merged data to 1 to be merged into final sorted arr
        if (my_rank == 1) {
            MPI_Send(&chunkSize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            MPI_Ssend(arrChunk, chunkSize, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
            freeArrMem(arrChunk, chunkSize);
        } else if (my_rank == 3) {
            MPI_Send(&chunkSize, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
            MPI_Ssend(arrChunk, chunkSize, MPI_FLOAT, 2, 0, MPI_COMM_WORLD);
            freeArrMem(arrChunk, chunkSize);
        } else if (my_rank == 2) {
            int chunkSize2;
            MPI_Recv(&chunkSize2, 1, MPI_INT, 3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            arrChunk2 = malloc(chunkSize2 * sizeof(float));
            MPI_Recv(arrChunk2, chunkSize2, MPI_FLOAT, 3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            int mergedArrSize = chunkSize + chunkSize2;
            mergedArr = malloc((mergedArrSize) * sizeof(float));
            merge(arrChunk, arrChunk2, chunkSize, chunkSize2, mergedArr);
            freeArrMem(arrChunk, chunkSize);
            freeArrMem(arrChunk2, chunkSize2);
            
            MPI_Send(&mergedArrSize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            MPI_Ssend(mergedArr, mergedArrSize, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
            freeArrMem(mergedArr, mergedArrSize);
        }
    }

    /* Tear down the communication infrastructure */
    MPI_Finalize();
    return 0;
}

void bubblesort(float* arrChunk, int chunkSize) {
    // Var that tracks if the array is sorted or not
    bool sorted = false;

    while (!sorted) {
        // Assumes that the array is sorted initially
        sorted = true;

        for (int i = 0; i < chunkSize - 1; i++) {
            if (arrChunk[i] > arrChunk[i+1]) {
                // Swaps elements at positions [i] and [i+1]
                float temp = arrChunk[i];
                arrChunk[i] = arrChunk[i+1];
                arrChunk[i+1] = temp;

                // If anything was not sorted, sorted is set to false and the while loop iterates through again
                sorted = false;
            }
        }
    }
}

void merge(float* arr1, float* arr2, int size1, int size2, float* mergedArr) {
    int i = 0;
    int j = 0;
    int k = 0;
    
    // Iterate through each array, comparing until one array is finished
    while (i < size1 && j < size2) {
        if (arr1[i] <= arr2[j]) {
            mergedArr[k] = arr1[i];
            i++;
        } else {
            mergedArr[k] = arr2[j];
            j++;
        }
        k++;
    }

    // Add the rest of the other array onto the end of mergedArr
    while (i < size1) {
        mergedArr[k] = arr1[i];
        i++;
        k++;
    }

    while (j < size2) {
        mergedArr[k] = arr2[j];
        j++;
        k++;
    }
}

// Frees up the memory for the array that was dynamically allocated onto the heap 
void freeArrMem(float* arr, int arrSize) {
    if (arr != NULL) {
        free(arr);
    }
}