/* Author: Kyle Tranfaglia
*  COSC420: Parallel Database Query Processing System Project
*  Last updated 11/18/24
*  This program contains search utility functions to return a list of all entires that meet a certain criteria
*/



#include "searchUtility.h"

// Generalized filter function to return a filtered CarContainer
CarContainer* find_all(CarContainer* car_data, const void* value, ComparisonOperation op, ComparisonObject obj) {
    // Allocate space for the result container and initialize the CarContainer object
    CarContainer* results = (CarContainer*)malloc(sizeof(CarContainer));
    int capacity = 32;
    results->array = (Car*)malloc(capacity * sizeof(Car));
    results->size = 0;

    // Traverse each car in the container
    for (int i = 0; i < car_data->size; i++) {
        Car* current = &car_data->array[i];  // Extract a car

        //printf("%d\n", condition(current, value, op, obj)); // current 0
        // Check if the car meets the condition
        if (condition(current, value, op, obj)) {
            // Resize array if capacity has been met
            if (results->size == capacity) {
                capacity *= 2;
                results->array = (Car*)realloc(results->array, capacity * sizeof(Car));
            }
            results->array[results->size++] = copyCar(*current);
            //results->array[results->size++] = *current; // Add car to the result
        }
    }

    return results;  // Return the filtered container
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
    //printf("Operations is %d\n", obj);
    switch (obj) {
        case ID:
            return compare_int(car->ID, *(int*)value, op);
        case PRICE:
            return compare_int(car->Price, *(int*)value, op);
        case MAKE:
            return compare_int(car->YearMake, *(int*)value, op);
        case MODEL:
            //printf("Model: %s & Value: %s\n", car->Model, (char*)value); //PROBLEM ITS IN QUOTATIONS
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
    result->array = (Car*)malloc((array1->size + array2->size) * sizeof(Car));
    result->size = 0;


    //printf("Array 2 Size: %d\n", array2->size);
    // Add all cars from the first array to the result
    for (int i = 0; i < array1->size; i++) {
        result->array[result->size++] = array1->array[i];
    }

    // Add cars from the second array only if they are not already in the result
    for (int i = 0; i < array2->size; i++) {
        int found = 0;
        // Check if the car already exists in the result
        for (int j = 0; j < result->size; j++) {
            if (array2->array[i].ID == result->array[j].ID) {
                found = 1;  // Mark as found
                break;
            }
        }
        // If not found, add the car to the result
        if (!found) {
            result->array[result->size++] = copyCar(array2->array[i]);
        }
    }
    return result;
}

// Intersection of two CarContainer arrays
CarContainer* intersect_arrays(CarContainer* array1, CarContainer* array2) {
    // Allocate space for the result container and initialize the CarContainer object
    CarContainer* result = (CarContainer*)malloc(sizeof(CarContainer));
    result->array = (Car*)malloc((array1->size < array2->size ? array1->size : array2->size) * sizeof(Car));
    result->size = 0;

    // Iterate through the first array
    for (int i = 0; i < array1->size; i++) {
        // Check if the current car is present in the second array
        for (int j = 0; j < array2->size; j++) {
            // If found, add the car to the result
            if (array1->array[i].ID == array2->array[j].ID) {
                
                result->array[result->size++] = copyCar(array2->array[j]);
                break;
            }
        }
    }
    return result;
}


