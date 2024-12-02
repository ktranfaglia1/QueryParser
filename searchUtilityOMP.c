/* Author: Kyle Tranfaglia
*  COSC420: Parallel Database Query Processing System Project
*  Last updated 12/01/24
*  This program contains search utility functions to return a list of all entires that meet a certain criteria
*/
#include "searchUtilityOMP.h"

// Generalized filter function to return a filtered CarContainer
CarContainer* find_all(CarContainer* car_data, const void* value, ComparisonOperation op, ComparisonObject obj) {
    // Allocate space for the result container and initialize the CarContainer object
    CarContainer* results = (CarContainer*)malloc(sizeof(CarContainer));
    int capacity = 64;
    results->array = (Car*)malloc(capacity * sizeof(Car));
    results->size = 0;
    results->capacity = capacity;

    // Parallel processing to find matching cars
    #pragma omp parallel
    {
        // Thread-local result container
        CarContainer local_result = {0};
        local_result.array = (Car*)malloc(capacity * sizeof(Car));
        local_result.size = 0;
        local_result.capacity = capacity;

        // Divide the work of filtering across threads
        #pragma omp for schedule(guided)
        for (int i = 0; i < car_data->size; i++) {
            // Check if the current car satisfies the condition
            if (condition(&car_data->array[i], value, op, obj)) {
                // Resize local result if needed
                if (local_result.size >= local_result.capacity) {
                    local_result.capacity *= 2;
                    local_result.array = (Car*)realloc(local_result.array, local_result.capacity * sizeof(Car));
                }
                local_result.array[local_result.size++] = copyCar(car_data->array[i]);
            }
        }

        // Merge thread-local results into the global result
        #pragma omp critical
        {
            // Resize global result if needed
            if (results->size + local_result.size > results->capacity) {
                results->capacity = (results->size + local_result.size) * 2;
                results->array = (Car*)realloc(results->array, results->capacity * sizeof(Car));
            }
            memcpy(results->array + results->size, local_result.array, local_result.size * sizeof(Car));
            results->size += local_result.size;
        }
        free(local_result.array);
    }

    return results;
}


// Compares two integers based on the specified comparison operation, given a and b for comparison
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


// Compares two strings using strcmp based on the specified comparison operation, given a and b for comparison
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


// Evaluates a condition for a specific car field using the specified comparison and comparison utility functions
int condition(const Car* car, const void* value, ComparisonOperation op, ComparisonObject obj) {
    switch (obj) {
        case ID:
            return compare_int(car->ID, *(int*)value, op);
        case PRICE:
            return compare_int(car->Price, *(int*)value, op);
        case MAKE:
            return compare_int(car->YearMake, *(int*)value, op);
        case MODEL:
            return compare_string(car->Model, (const char*)value, op);
        case COLOR:
            return compare_string(car->Color, (const char*)value, op);
        case DEALER:
            return compare_string(car->Dealer, (const char*)value, op);
        default:
            return 0;
    }
}


// Union of two CarContainer arrays
CarContainer* union_arrays(CarContainer* array1, CarContainer* array2) {
    // Allocate space for the result container and initialize the CarContainer object
    CarContainer* result = (CarContainer*)malloc(sizeof(CarContainer));
    int capacity = array1->size + array2->size;
    result->array = (Car*)malloc(capacity * sizeof(Car));
    result->size = 0;
    result->capacity = capacity;

    // Add all cars from array1 to the result (no duplicates in array1)
    for (int i = 0; i < array1->size; i++) {
        result->array[result->size++] = copyCar(array1->array[i]);
    }

    // Parallel processing for array2
    #pragma omp parallel
    {
        // Thread-local container for unique elements
        CarContainer local_result = {0};
        local_result.array = (Car*)malloc(capacity * sizeof(Car));
        local_result.size = 0;
        local_result.capacity = capacity;

        // Divide work among threads
        #pragma omp for schedule(guided)
        for (int i = 0; i < array2->size; i++) {
            int found = 0;

            // Check if the current car from array2 is already in the global result
            #pragma omp critical
            {
                for (int j = 0; j < result->size; j++) {
                    if (array2->array[i].ID == result->array[j].ID) {
                        found = 1;
                        break;
                    }
                }
            }

            // Add to thread-local results if not found
            if (!found) {
                local_result.array[local_result.size++] = copyCar(array2->array[i]);
            }
        }

        // Merge thread-local unique results into the global result
        #pragma omp critical
        {
            memcpy(result->array + result->size, local_result.array, local_result.size * sizeof(Car));
            result->size += local_result.size;
        }
        free(local_result.array);
    }

    return result;
}

// Intersection of two CarContainer arrays
CarContainer* intersect_arrays(CarContainer* array1, CarContainer* array2) {
    // Allocate space for the result container and initialize the CarContainer object
    CarContainer* result = (CarContainer*)malloc(sizeof(CarContainer));
    int capacity = 64;
    result->array = (Car*)malloc(capacity * sizeof(Car));
    result->size = result->capacity = 0;

    // Parallel processing to find the intersection
    #pragma omp parallel
    {
        // Allocate and initialize thread-local result container
        CarContainer local_result = {0};
        local_result.array = (Car*)malloc(capacity * sizeof(Car));
        local_result.size = 0;
        local_result.capacity = capacity;

        // Divide work among threads
        #pragma omp for schedule(guided)
        for (int i = 0; i < array1->size; i++) {
            for (int j = 0; j < array2->size; j++) {
                // Check for ID match
                if (array1->array[i].ID == array2->array[j].ID) {
                    // Resize thread-local array if necessary
                    if (local_result.size == local_result.capacity) {
                        local_result.capacity *= 2;
                        local_result.array = (Car*)realloc(local_result.array, capacity * sizeof(Car));
                    }
                    local_result.array[local_result.size++] = copyCar(array1->array[i]);
                    break;
                }
            }
        }

        // Merge thread-local results into the global result
        #pragma omp critical
        {
            if (result->size + local_result.size > capacity) {
                result->array = realloc(result->array, (result->size + local_result.size) * sizeof(Car));
            }
            memcpy(result->array + result->size, local_result.array, local_result.size * sizeof(Car));
            result->size += local_result.size;
        }
        free(local_result.array);
    }
    return result;
}