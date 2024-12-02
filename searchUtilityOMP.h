#ifndef SEARCHUTILITYOMP_H
#define SEARCHUTILITYOMP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "theDatabaser.h"

// Comparison operations
typedef enum {
    GREATER_THAN,
    GREATER_EQUAL,
    LESS_THAN,
    LESS_EQUAL,
    EQUAL_TO,
    NOT_EQUAL_TO,
} ComparisonOperation;

// Comparison objects
typedef enum {
    ID,
    MODEL,
    MAKE,
    COLOR,
    PRICE,
    DEALER,
} ComparisonObject;

// Function prototypes
int compare_int(int a, int b, ComparisonOperation op);
int compare_string(const char* a, const char* b, ComparisonOperation op);
int condition(const Car* car, const void* value, ComparisonOperation op, ComparisonObject obj);

CarContainer* find_all(CarContainer* car_data, const void* value, ComparisonOperation op, ComparisonObject obj);
CarContainer* union_arrays(CarContainer* array1, CarContainer* array2);
CarContainer* intersect_arrays(CarContainer* array1, CarContainer* array2);

#endif
