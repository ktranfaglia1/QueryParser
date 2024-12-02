/* Author: Kyle Tranfaglia
*  COSC420: Parallel Database Query Processing System Project
*  Last updated 12/01/24
*  This program contains a parallel search utility functions to return a list of all entires that meet a certain criteria using MPI
*/
#include "searchUtilityMPI.h"

#define TAG_DATA 0
#define TAG_RESULTS 1

// Generalized filter function using MPI to return a filtered CarContainer using MPI
CarContainer* find_all(CarContainer* car_data, const void* value, ComparisonOperation op, ComparisonObject obj) {
    // Initialize MPI environment
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Split the work across processes
    int local_start = (rank * car_data->size) / size;
    int local_end = ((rank + 1) * car_data->size) / size;

    // Local results container
    CarContainer* local_results = (CarContainer*)malloc(sizeof(CarContainer));
    local_results->array = (Car*)malloc((local_end - local_start) * sizeof(Car));
    local_results->size = 0;

    // Process its portion of the data
    for (int i = local_start; i < local_end; i++) {
        Car* current = &car_data->array[i];
        if (condition(current, value, op, obj)) {
            local_results->array[local_results->size++] = copyCar(*current);
        }
    }

     // Non-blocking send of the size of the local results to rank 0
    MPI_Request send_request;
    MPI_Isend(&local_results->size, 1, MPI_INT, 0, TAG_RESULTS, MPI_COMM_WORLD, &send_request);

    // Rank 0 prepares to receive results from all processes
    int* recv_sizes = NULL;
    int* displs = NULL;
    CarContainer* final_results = NULL;

    // Master process
    if (rank == 0) {
        // Allocate memory for sizes and displacements
        recv_sizes = (int*)malloc(size * sizeof(int));
        displs = (int*)malloc(size * sizeof(int));

        // Initialize the final results container
        final_results = (CarContainer*)malloc(sizeof(CarContainer));
        final_results->array = (Car*)malloc(car_data->size * sizeof(Car));
        final_results->size = 0;

        // Non-blocking receive for sizes of local results from all processes
        MPI_Request* recv_requests = (MPI_Request*)malloc(size * sizeof(MPI_Request));
        for (int i = 0; i < size; i++) {
            MPI_Irecv(&recv_sizes[i], 1, MPI_INT, i, TAG_RESULTS, MPI_COMM_WORLD, &recv_requests[i]);
        }
        MPI_Waitall(size, recv_requests, MPI_STATUSES_IGNORE);  // Wait for all size information to be received

        // Calculate displacements for the gathered data
        int total_size = 0;
        for (int i = 0; i < size; i++) {
            displs[i] = total_size;
            total_size += recv_sizes[i];
        }
        final_results->size = total_size;

        // Receive the actual data from all processes
        MPI_Irecv(final_results->array, total_size * sizeof(Car), MPI_BYTE, MPI_ANY_SOURCE, TAG_RESULTS, MPI_COMM_WORLD, &send_request);
        free(recv_requests);
    }
    MPI_Wait(&send_request, MPI_STATUS_IGNORE);  // Ensure non-blocking send completes before local process continues

    // Free resources
    free(local_results->array);
    free(local_results);
    if (rank == 0) {
        free(recv_sizes);
        free(displs);
    }

    return final_results;
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


// Union of two CarContainer arrays using MPI
CarContainer* union_arrays(CarContainer* array1, CarContainer* array2) {
    // Initialize MPI environment
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Determine the portion of array2 to be processed by each rank
    CarContainer* local_array2 = NULL;
    int chunk_size = (array2->size + size - 1) / size;  // Divide array2 into chunks
    int start = rank * chunk_size;  // Start index for the current process
    int end = (rank + 1) * chunk_size;  // End index (exclusive)
    if (end > array2->size) 
        end = array2->size;  // Ensure the end index does not exceed the array size

    // Create a local container for this process's portion of array2
    local_array2 = (CarContainer*)malloc(sizeof(CarContainer));
    local_array2->array = &array2->array[start];
    local_array2->size = end - start;

    // Initialize a container for the local results
    CarContainer* local_results = (CarContainer*)malloc(sizeof(CarContainer));
    local_results->array = (Car*)malloc(array2->size * sizeof(Car));
    local_results->size = 0;

    // Perform the local computation for union
    for (int i = 0; i < local_array2->size; i++) {
        int found = 0;
        for (int j = 0; j < array1->size; j++) {
            // Check for matching IDs
            if (local_array2->array[i].ID == array1->array[j].ID) {
                found = 1;
                break;
            }
        }
        // If not found, add the car to the local results
        if (!found) {
            local_results->array[local_results->size++] = copyCar(local_array2->array[i]);
        }
    }

    // Start a non-blocking gather of the sizes of local results to rank 0
    int local_size = local_results->size;
    int* recv_sizes = NULL;
    if (rank == 0) {
        recv_sizes = (int*)malloc(size * sizeof(int));
    }
    MPI_Request size_request;
    MPI_Igather(&local_size, 1, MPI_INT, recv_sizes, 1, MPI_INT, 0, MPI_COMM_WORLD, &size_request);

    // Perform final union on rank 0
    CarContainer* final_results = NULL;
    if (rank == 0) {
        final_results = (CarContainer*)malloc(sizeof(CarContainer));
        final_results->array = (Car*)malloc((array1->size + array2->size) * sizeof(Car));
        final_results->size = array1->size;
        memcpy(final_results->array, array1->array, array1->size * sizeof(Car));
    }
    MPI_Wait(&size_request, MPI_STATUS_IGNORE);  // Wait for the gather of sizes to complete

    // Start a non-blocking gather of the local results arrays to rank 0
    MPI_Request gather_request;
    int total_size = 0;
    int* displs = NULL;
    if (rank == 0) {
        displs = (int*)malloc(size * sizeof(int));  // Allocate displacements array
        for (int i = 0; i < size; i++) {
            displs[i] = total_size;
            total_size += recv_sizes[i];
        }
        final_results->size = total_size;
    }
    MPI_Igatherv(local_results->array, local_results->size * sizeof(Car), MPI_BYTE,
                 final_results ? final_results->array + array1->size : NULL,
                 recv_sizes, displs, MPI_BYTE, 0, MPI_COMM_WORLD, &gather_request);

    MPI_Wait(&gather_request, MPI_STATUS_IGNORE);  // Wait for the gather of arrays to complete

    // Cleanup dynamically allocated memory
    free(local_results->array);
    free(local_results);
    free(local_array2);
    if (rank == 0) {
        free(recv_sizes);
        free(displs);
    }

    return final_results;
}

// Intersect of two CarContainer arrays using MPI
CarContainer* intersect_arrays(CarContainer* array1, CarContainer* array2) {
    // Initialize MPI environment
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Determine the portion of array1 to be processed by this process
    int local_start = (rank * array1->size) / size;
    int local_end = ((rank + 1) * array1->size) / size;

    MPI_Bcast(&array2->size, 1, MPI_INT, 0, MPI_COMM_WORLD);  // Broadcast the size of array2 from rank 0 to all processes

    // Allocate memory for array2 on non-master processes and broadcast its contents
    if (rank != 0) {
        array2->array = (Car*)malloc(array2->size * sizeof(Car));
    }
    MPI_Bcast(array2->array, array2->size * sizeof(Car), MPI_BYTE, 0, MPI_COMM_WORLD);

    // Initialize a container for the local results
    CarContainer* local_results = (CarContainer*)malloc(sizeof(CarContainer));
    local_results->array = (Car*)malloc((local_end - local_start) * sizeof(Car));
    local_results->size = 0;

    // Perform the local computation for intersection
    for (int i = local_start; i < local_end; i++) {
        for (int j = 0; j < array2->size; j++) {
            // Check for matching IDs
            if (array1->array[i].ID == array2->array[j].ID) {
                local_results->array[local_results->size++] = copyCar(array1->array[i]);
                break;
            }
        }
    }

    // Gather the sizes of local results to rank 0
    int local_size = local_results->size;
    int* recv_sizes = NULL;
    if (rank == 0) {
        recv_sizes = (int*)malloc(size * sizeof(int));
    }
    MPI_Request size_request;
    MPI_Igather(&local_size, 1, MPI_INT, recv_sizes, 1, MPI_INT, 0, MPI_COMM_WORLD, &size_request);

    // Rank 0 prepares to merge the results
    CarContainer* final_results = NULL;
    if (rank == 0) {
        final_results = (CarContainer*)malloc(sizeof(CarContainer));
        final_results->array = (Car*)malloc(array1->size * sizeof(Car));
    }
    MPI_Wait(&size_request, MPI_STATUS_IGNORE);  // Wait for the gather of sizes to complete

    // Gather the local results arrays into rank 0
    MPI_Request gather_request;
    int* displs = NULL;
    if (rank == 0) {
        int total_size = 0;
        displs = (int*)malloc(size * sizeof(int));  // Allocate displacements array
        for (int i = 0; i < size; i++) {
            displs[i] = total_size;
            total_size += recv_sizes[i];
        }
        final_results->size = total_size;
    }
    MPI_Igatherv(local_results->array, local_results->size * sizeof(Car), MPI_BYTE,
                 final_results ? final_results->array : NULL,
                 recv_sizes, displs, MPI_BYTE, 0, MPI_COMM_WORLD, &gather_request);

    MPI_Wait(&gather_request, MPI_STATUS_IGNORE);  // Wait for the gather of arrays to complete

    // Cleanup dynamically allocated memory
    free(local_results->array);
    free(local_results);
    if (rank == 0) {
        free(recv_sizes);
        free(displs);
    }
    if (rank != 0) {
        free(array2->array);
    }

    return final_results;
}