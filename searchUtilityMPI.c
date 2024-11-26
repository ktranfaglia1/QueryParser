/* Author: Kyle Tranfaglia
*  COSC420: Parallel Database Query Processing System Project
*  Last updated 11/26/24
*  This program contains a parallel search utility functions to return a list of all entires that meet a certain criteria using MPI
*/
#include "searchUtilityMPI.h"

#define TAG_DATA 0
#define TAG_RESULTS 1

// Generalized filter function using MPI to return a filtered CarContainer
CarContainer* find_all(CarContainer* car_data, const void* value, ComparisonOperation op, ComparisonObject obj) {
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

    // Gather results from all processes on rank 0
    CarContainer* final_results = NULL;
    if (rank == 0) {
        final_results = (CarContainer*)malloc(sizeof(CarContainer));
        final_results->array = (Car*)malloc(car_data->size * sizeof(Car));
        final_results->size = 0;
    }

    int* recv_sizes = NULL;
    int* displs = NULL;
    if (rank == 0) {
        recv_sizes = (int*)malloc(size * sizeof(int));
        displs = (int*)malloc(size * sizeof(int));
    }

    // Gather sizes of results
    MPI_Gather(&local_results->size, 1, MPI_INT, recv_sizes, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        int total_size = 0;
        for (int i = 0; i < size; i++) {
            displs[i] = total_size;
            total_size += recv_sizes[i];
        }
        final_results->size = total_size;
    }

    // Gather all data
    MPI_Gatherv(
        local_results->array,
        local_results->size * sizeof(Car),
        MPI_BYTE,
        final_results ? final_results->array : NULL,
        recv_sizes,
        displs,
        MPI_BYTE,
        0,
        MPI_COMM_WORLD);

    // Free memory
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
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Rank 0 distributes the data for array2
    CarContainer* local_array2 = NULL;
    int local_size = 0;
    if (rank == 0) {
        int chunk_size = (array2->size + size - 1) / size;
        for (int i = 1; i < size; i++) {
            int start = i * chunk_size;
            int end = (i + 1) * chunk_size;
            if (end > array2->size) end = array2->size;

            CarContainer temp;
            temp.array = &array2->array[start];
            temp.size = end - start;

            sendCarContainer(&temp, i, TAG_DATA, MPI_COMM_WORLD);
        }
        local_array2 = (CarContainer*)malloc(sizeof(CarContainer));
        local_array2->array = array2->array;
        local_array2->size = chunk_size < array2->size ? chunk_size : array2->size;
    } else {
        local_array2 = recvCarContainer(0, TAG_DATA, MPI_COMM_WORLD);
    }

    // Find local results
    CarContainer* local_results = (CarContainer*)malloc(sizeof(CarContainer));
    local_results->array = (Car*)malloc(local_array2->size * sizeof(Car));
    local_results->size = 0;

    for (int i = 0; i < local_array2->size; i++) {
        int found = 0;
        for (int j = 0; j < array1->size; j++) {
            if (local_array2->array[i].ID == array1->array[j].ID) {
                found = 1;
                break;
            }
        }
        if (!found) {
            local_results->array[local_results->size++] = copyCar(local_array2->array[i]);
        }
    }

    // Gather results
    CarContainer* final_results = NULL;
    if (rank == 0) {
        final_results = (CarContainer*)malloc(sizeof(CarContainer));
        final_results->array = (Car*)malloc((array1->size + array2->size) * sizeof(Car));
        final_results->size = array1->size;

        // Copy array1 to final results
        memcpy(final_results->array, array1->array, array1->size * sizeof(Car));
    }

    MPI_Gather(local_results->array, local_results->size * sizeof(Car), MPI_BYTE,
               final_results ? final_results->array + final_results->size : NULL,
               local_results->size * sizeof(Car), MPI_BYTE,
               0, MPI_COMM_WORLD);

    // Cleanup
    free(local_results->array);
    free(local_results);
    free(local_array2->array);
    free(local_array2);

    return final_results;
}


CarContainer* intersect_arrays(CarContainer* array1, CarContainer* array2) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Each process will handle a portion of array1
    int local_start = (rank * array1->size) / size;
    int local_end = ((rank + 1) * array1->size) / size;

    // Send array2 to all processes (broadcast)
    MPI_Bcast(&array2->size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank != 0) {
        array2->array = (Car*)malloc(array2->size * sizeof(Car));
    }
    MPI_Bcast(array2->array, array2->size * sizeof(Car), MPI_BYTE, 0, MPI_COMM_WORLD);

    // Each process computes the intersection for its portion of array1
    CarContainer* local_results = (CarContainer*)malloc(sizeof(CarContainer));
    local_results->array = (Car*)malloc(array1->size * sizeof(Car));  // Max possible size
    local_results->size = 0;

    for (int i = local_start; i < local_end; i++) {
        for (int j = 0; j < array2->size; j++) {
            if (array1->array[i].ID == array2->array[j].ID) {
                local_results->array[local_results->size++] = copyCar(array1->array[i]);
                break;  // Move to the next car in array1
            }
        }
    }

    // Gather all results on rank 0
    CarContainer* final_results = NULL;
    if (rank == 0) {
        final_results = (CarContainer*)malloc(sizeof(CarContainer));
        final_results->array = (Car*)malloc(array1->size * sizeof(Car));  // Max possible size
        final_results->size = 0;
    }

    int* recv_sizes = NULL;
    int* displs = NULL;
    if (rank == 0) {
        recv_sizes = (int*)malloc(size * sizeof(int));
        displs = (int*)malloc(size * sizeof(int));
    }

    // Gather sizes of local results
    MPI_Gather(&local_results->size, 1, MPI_INT, recv_sizes, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        int total_size = 0;
        for (int i = 0; i < size; i++) {
            displs[i] = total_size;
            total_size += recv_sizes[i];
        }
        final_results->size = total_size;
    }

    // Gather all local results into final_results on rank 0
    MPI_Gatherv(
        local_results->array,
        local_results->size * sizeof(Car),
        MPI_BYTE,
        final_results ? final_results->array : NULL,
        recv_sizes,
        displs,
        MPI_BYTE,
        0,
        MPI_COMM_WORLD);

    // Cleanup
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


void sendCarContainer(CarContainer* container, int dest, int tag, MPI_Comm comm) {
    // Send the size of the container
    MPI_Send(&(container->size), 1, MPI_INT, dest, tag, comm);

    // Send the data for each car in the container
    for (int i = 0; i < container->size; i++) {
        Car* car = &(container->array[i]);
        
        // Send integer fields
        MPI_Send(&(car->ID), 1, MPI_INT, dest, tag, comm);
        MPI_Send(&(car->Price), 1, MPI_INT, dest, tag, comm);
        MPI_Send(&(car->YearMake), 1, MPI_INT, dest, tag, comm);

        // Send string fields (length + content)
        int model_len = strlen(car->Model) + 1; // Include null-terminator
        MPI_Send(&model_len, 1, MPI_INT, dest, tag, comm);
        MPI_Send(car->Model, model_len, MPI_CHAR, dest, tag, comm);

        int color_len = strlen(car->Color) + 1;
        MPI_Send(&color_len, 1, MPI_INT, dest, tag, comm);
        MPI_Send(car->Color, color_len, MPI_CHAR, dest, tag, comm);

        int dealer_len = strlen(car->Dealer) + 1;
        MPI_Send(&dealer_len, 1, MPI_INT, dest, tag, comm);
        MPI_Send(car->Dealer, dealer_len, MPI_CHAR, dest, tag, comm);
    }
}

void sendCarContainer(CarContainer* container, int dest, int tag, MPI_Comm comm) {
    // Send the size of the container
    MPI_Send(&(container->size), 1, MPI_INT, dest, tag, comm);

    // Send the data for each car in the container
    for (int i = 0; i < container->size; i++) {
        Car* car = &(container->array[i]);
        
        // Send integer fields
        MPI_Send(&(car->ID), 1, MPI_INT, dest, tag, comm);
        MPI_Send(&(car->Price), 1, MPI_INT, dest, tag, comm);
        MPI_Send(&(car->YearMake), 1, MPI_INT, dest, tag, comm);

        // Send string fields (length + content)
        int model_len = strlen(car->Model) + 1; // Include null-terminator
        MPI_Send(&model_len, 1, MPI_INT, dest, tag, comm);
        MPI_Send(car->Model, model_len, MPI_CHAR, dest, tag, comm);

        int color_len = strlen(car->Color) + 1;
        MPI_Send(&color_len, 1, MPI_INT, dest, tag, comm);
        MPI_Send(car->Color, color_len, MPI_CHAR, dest, tag, comm);

        int dealer_len = strlen(car->Dealer) + 1;
        MPI_Send(&dealer_len, 1, MPI_INT, dest, tag, comm);
        MPI_Send(car->Dealer, dealer_len, MPI_CHAR, dest, tag, comm);
    }
}
