#ifndef SEARCHUTILITYMPI_H
#define SEARCHUTILITYMPI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "theDatabaser.h"



// Function prototypes
int compare_int(int a, int b, ComparisonOperation op);
int compare_string(const char* a, const char* b, ComparisonOperation op);
int condition(const Car* car, const void* value, ComparisonOperation op, ComparisonObject obj);

CarContainer* find_all(CarContainer* car_data, const void* value, ComparisonOperation op, ComparisonObject obj);
CarContainer* union_arrays(CarContainer* array1, CarContainer* array2);
CarContainer* intersect_arrays(CarContainer* array1, CarContainer* array2);

// Helper functions
void sendCarContainer(CarContainer* container, int dest, int tag, MPI_Comm comm);
CarContainer* recvCarContainer(int source, int tag, MPI_Comm comm);

#endif