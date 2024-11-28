/* Author: Kyle Tranfaglia
*  COSC420: Parallel Database Query Processing System Project
*  Last updated 11/18/24
*  This program contains search utility functions to return a list of all entires that meet a certain criteria
*/
#include "searchUtilityOMP.h"

// Generalized filter function to return a filtered CarContainer
CarContainer* find_all(CarContainer* car_data, const void* value, ComparisonOperation op, ComparisonObject obj) {
    // Allocate space for the result container and initialize the CarContainer object
    CarContainer* results = (CarContainer*)malloc(sizeof(CarContainer));
    int capacity = 32;
    results->array = (Car*)malloc(capacity * sizeof(Car));
    results->size = 0;

    // Thread-local result containers
    CarContainer* local_results = (CarContainer*)calloc(omp_get_max_threads(), sizeof(CarContainer));
    #pragma omp parallel shared(totalCapacity)
    {
        int tid = omp_get_thread_num();
        local_results[tid].array = (Car*)malloc(capacity * sizeof(Car));
        local_results[tid].size = 0;

        // Divide the work of filtering across threads
        #pragma omp for
        for (int i = 0; i < car_data->size; i++) {
            // Check if the current car satisfies the condition
            if (condition(&car_data->array[i], value, op, obj)) {
                // Resize array if capacity is met
                if (local_results[tid].size == capacity) {
                    capacity *= 2;
                    local_results[tid].array = (Car*)realloc(local_results[tid].array, capacity * sizeof(Car));
                }
                // Use copyCar to ensure a deep copy of the car object
                local_results[tid].array[local_results[tid].size++] = copyCar(car_data->array[i]);
            }
        }
        

    }
    
    capacity = 32;

    // Merge thread-local results into the final results
    for (int i = 0; i < omp_get_max_threads(); i++) {
        
        for (int j = 0; j < local_results[i].size; j++) {
            // Resize the global result array if necessary
            if (results->size == capacity) {
                capacity *= 2;
                results->array = (Car*)realloc(results->array, capacity * sizeof(Car));
            }
            results->array[results->size++] = copyCar(local_results[i].array[j]); // Use copyCar to ensure correct memory handling
        }
        free(local_results[i].array);  // Free thread-local arrays
    }
    free(local_results);  // Free the thread-local results container

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

    // Add all cars from array1 to the result (no duplicates in array1)
    for (int i = 0; i < array1->size; i++) {
        result->array[result->size++] = copyCar(array1->array[i]);
    }

    // Allocate thread-local result containers to avoid race conditions
    int max_threads = omp_get_max_threads();
    CarContainer* local_results = (CarContainer*)malloc(max_threads * sizeof(CarContainer));
    for (int t = 0; t < max_threads; t++) {
        local_results[t].array = (Car*)malloc(capacity * sizeof(Car));  // Allocate local arrays for threads
        local_results[t].size = 0;
    }

    // Parallel processing of array2
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        for (int i = 0; i < array2->size; i++) {
            int found = 0;

            // Check if the current car from array2 is already in array1
            for (int j = 0; j < array1->size; j++) {
                if (array2->array[i].ID == array1->array[j].ID) {
                    found = 1;
                    break;
                }
            }

            // Add to thread-local results if not found
            if (!found) {
                local_results[tid].array[local_results[tid].size++] = copyCar(array2->array[i]);
            }
        }
    }

    // Merge thread-local results into the final result
    for (int t = 0; t < max_threads; t++) {
        for (int i = 0; i < local_results[t].size; i++) {
            int found = 0;

            // Check if the car is already in the global result
            for (int j = 0; j < result->size; j++) {
                if (local_results[t].array[i].ID == result->array[j].ID) {
                    found = 1;
                    break;
                }
            }

            // Add to the global result if not found
            if (!found) {
                if (result->size == capacity) {
                    capacity *= 2;
                    result->array = (Car*)realloc(result->array, capacity * sizeof(Car));
                }
                result->array[result->size++] = copyCar(local_results[t].array[i]);
            }
        }
        free(local_results[t].array);  // Free thread-local arrays
    }
    free(local_results);  // Free thread-local result containers
    return result;
}

// Intersection of two CarContainer arrays
CarContainer* intersect_arrays(CarContainer* array1, CarContainer* array2) {
    // Allocate space for the result container and initialize the CarContainer object
    CarContainer* result = (CarContainer*)malloc(sizeof(CarContainer));
    int capacity = 32;
    result->array = (Car*)malloc(capacity * sizeof(Car));
    result->size = 0;

    // Parallel processing to find the intersection
    #pragma omp parallel
    {
        // Allocate and initialize thread-local result container
        CarContainer local_result;
        local_result.array = (Car*)malloc(capacity * sizeof(Car));
        local_result.size = 0;

        // Divide work among threads
        #pragma omp for
        for (int i = 0; i < array1->size; i++) {
            for (int j = 0; j < array2->size; j++) {
                // Check for ID match
                if (array1->array[i].ID == array2->array[j].ID) {
                    // Resize thread-local array if necessary
                    if (local_result.size == capacity) {
                        capacity *= 2;
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
            for (int i = 0; i < local_result.size; i++) {
                if (result->size == capacity) {
                    capacity *= 2;
                    result->array = (Car*)realloc(result->array, capacity * sizeof(Car));
                }
                result->array[result->size++] = copyCar(local_result.array[i]);
            }
        }
        free(local_result.array);  // Free thread-local array
    }
    return result;
}