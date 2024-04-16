# Makefile for mpi program
# Author: Dante Webber
# S.No: WBBDAN003

CC=mpicc # Compiler used is mpi
# CCFLAGS= # Put any compiler flags here, currently unnecessary
SOURCES=mpi_sort.c # Source files to be compiled
OBJECTS=mpi_sort.o # Object files to be compiled
RC=mpirun # Run command used
RUNFLAGS=-np 4 # Specifies the number of processes to be used
T_FLAGS =--use-hwthread-cpus -np 8 # Makes mpi use the number of threads (to include hyper-threads) instead of actual CPU cores

mpi_sort: $(SOURCES)
	$(CC) $(SOURCES) -o mpi_sort

randarrgen: rnd_num_array_gen.c
	gcc rnd_num_array_gen.c -c
	gcc rnd_num_array_gen.o -o randarrgen

seq_sort: sequential_sort.c
	gcc sequential_sort.c -c
	gcc sequential_sort.o -o seq_sort

run:
	$(RC) $(RUNFLAGS) ./mpi_sort $(filter-out $@,$(MAKECMDGOALS))
%:
	@:

run_threads:
	$(RC) $(T_FLAGS) mpi_sort

clean:
	rm -f *.o mpi_sort
