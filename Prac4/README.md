# Prac 4 Read Me
This repo conatains source files and a Makefile that automates all compilation and running of the main mpi_sort program.
To compile:
  - "make" : This will compile the main mpi sort program
  - "make randarrgen" : This will compile the program that will generate arrays containing random float values.
  - "make seq_sort" : This will compile the sequential sort program that runs a sequential bubble sort.

To run:
  - "Make run <input.csv> <output.csv>" : will run the mpi_sort program with 4 processes, must specify input and output csv file names
  - "./randarrgen <arrSize>" : This will run the random array generator that will write a csv file with name arrSizexarrSize.csv filled with random float numbers
  - "./seq_sort <input.csv> <output.csv>" : This will run the sequential bubble sort on the input.csv file and output the sorted array to the output.csv file

"make clean" will remove all binary files and the mpi_sort executable from the current directory.
