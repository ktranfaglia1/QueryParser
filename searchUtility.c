/* Author: Kyle Tranfaglia
*  COSC420: Parallel Database Query Processing System Project
*  Last updated 11/18/24
*  This program contains search utility functions to return a list of all entires that meet a certain criteria
*/
#include <stdio.h>
#include <stdlib.h>
#include "theDatabaser.c"

// Comparison operations
typedef enum {
    GREATER_THAN,
    GREATER_EQUAL,
    LESS_THAN,
    LESS_EQUAL,
    EQUAL_TO,
    NOT_EQUAL_TO,
} ComparisonOperation;

// Comparison object
typedef enum {
    ID,
    MODEL,
    MAKE,
    COLOR,
    PRICE,
    DEALER
} ComparisonObject;


// Generalized filter function to return an array and count
CarContainer* find_all(CarContainer* car_data, const void* value, ComparisonOperation op, ComparisonObject obj) {
    // Allocate space for the result array (initially arbitrary, grows if needed)
    int capacity = 32;
    CarContainer* results = (CarContainer*)malloc(capacity * sizeof(Car*));
    int* result_count = 0;

    // Traverse each bucket in the original table
    for (int i = 0; i < car_data->size; i++) {
        Car* current = car_data[i];

        // Traverse the linked list in the current bucket
        while (current != NULL) {
            if (condition(current, value, op, obj)) {
                // Resize array if needed
                if (*result_count == capacity) {
                    capacity *= 2;
                    results = (Car**)realloc(results, capacity * sizeof(Car*));
                }
                results[(*result_count)++] = current;  // Add pointer to car to the results
            }
        }
    }
    results->size = *result_count;
    return results;  // Return the array of matching entries
}


int compare_int(int a, int b, ComparisonOperation op) {
    switch (op) {
        case GREATER_THAN: 
            return a > b;
        case GREATER_EQUAL: 
            return a >= b;
        case LESS_THAN: 
            return a < b;
        case LESS_EQUAL: 
            return a <= b;
        case EQUAL_TO: 
            return a == b;
        case NOT_EQUAL_TO: 
            return a != b;
        default: 
            return 0;
    }
}


int compare_string(const char* a, const char* b, ComparisonOperation op) {
    switch (op) {
        case EQUAL_TO: 
            return strcmp(a, b) == 0;
        case NOT_EQUAL_TO: 
            return strcmp(a, b) != 0;
        default: 
            return 0;
    }
}


int condition(const Car* car, const void* value, ComparisonOperation op, ComparisonObject obj) {
    switch (obj) {
        case ID:
            return compare_int(car->id, *(int*)value, op);

        case PRICE:
            return compare_int(car->price, *(int*)value, op);

        case MAKE:
            return compare_int(car->make_year, *(int*)value, op);

        case MODEL:
            return compare_string(car->model, (const char*)value, op);

        case COLOR:
            return compare_string(car->color, (const char*)value, op);

        case DEALER:
            return compare_string(car->dealer, (const char*)value, op);

        default:
            return 0;
    }
}


// Union 
CarContainer* union_arrays(CarContainer* array1, CarContainer* array2) {
    // Allocate space for the union of two arrays
    int capacity = array1->size + array2->size;
    CarContainer* result = (CarContainer*)malloc(capacity * sizeof(Car*));
    int* result_count = 0;

    // Add all items from array1
    for (int i = 0; i < array1->size; i++) {
        result[(*result_count)++] = array1[i];
    }

    // Add items from array2 if they are not already in the result array
    for (int i = 0; i < array2->size; i++) {
        int found = 0;
        for (int j = 0; j < *result_count && !found; j++) {
            if (array2[i]->id == result[j]->id) {
                found = 1;
            }
        }
        if (!found) {
            result[(*result_count)++] = array2[i];
        }
    }
    result->size = *result_count
    return result;
}


CarContainer* intersect_arrays(CarContainer* array1, CarContainer* array2) {
    int capacity = (array1->size < array2->size) ? array1->size : array2->size;
    CarContainer* result = (CarContainer*)malloc(capacity * sizeof(Car*));
    int* result_count = 0;

    for (int i = 0; i < array1->size; i++) {
        for (int j = 0; j < array2->size; j++) {
            if (array1[i]->id == array2[j]->id) {
                result[(*result_count)++] = array1[i];
                break;
            }
        }
    }
    result->size = *result_count
    return result;
}