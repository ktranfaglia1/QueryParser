#include <stdio.h>
#include <time.h>
#include <mpi.h>
#include "theDatabaser.h"
#include "QueryParsing.h"
#include "parse.h"
#include "unistd.h"


int main(int argc, char** argv) {

    
    int numQueries = 0;
    //Turns Parameter into Parsed Array
    char*** parameters = getParams(argc, argv, &numQueries); 
    
    CarContainer** databases = (CarContainer**)malloc(sizeof(CarContainer*) * numQueries);
    opTuple** postFix  = (opTuple**)malloc(sizeof(opTuple*) * numQueries);


    //Creates a separate database and post fix for each query

    
    MPI_Init(&argc, &argv);

    double start_time = MPI_Wtime();

    for(int i = 0; i < numQueries; i++){
        databases[i] = createDatabase(parameters[i * 3 + 1][1]);

         //Converts Operations to postFix
        postFix[i] = where(parameters[i * 3 + 2]);
    }


    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get the rank of the process
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int i = rank % numQueries;
    //for(int i = rank; i < numQueries; i+= size)
    {
        //printf("Start\n");
        int queryID[size];
        
       // printf("Ptr. %p, i: %p\n", queryID, &i);
        MPI_Allgather(&i, 1, MPI_INT, queryID, 1, MPI_INT, MPI_COMM_WORLD);
        
        
        int queryCount = 0;
        //printf("Ptr. %p\n", queryID);

        //printf("Checkpoint\n");
        for(int j = 0; j < size; j++){
            queryCount += (queryID[j] == i);
        }
        //printf("Checkpoint 2\n");
        

        //printf("\nQuery %d\n\n", i);
        
        CarContainer* database = (CarContainer*)malloc(sizeof(CarContainer));
        
        

        database->size = databases[i]->size / queryCount;
        database->array = (Car*)malloc(sizeof(Car) * database->size);

        

        int counter = 0;
        for(int j = database->size * (rank % queryCount); j < database->size * (rank % queryCount + 1); j++){
            database->array[counter] = databases[i]->array[j];
            counter++;
        }

        //printf("Checkpoint\n");
        
        //Runs Operations Accordingly
        database = callOperations(database, postFix[i]); 

        

        //Outputs Database with proper columns
        //printf("Thread %d\n", rank);
        //sleep(i * 1);
        //if(rank < numQueries){
        //    printf("Query %d\n\n", i);
        //}
        //else{
        //    sleep(.1);
        //}
        printDatabase(database, parameters[i * 3]);

        //2>/dev/null
        
        //Frees Memory
        freeDatabase(database);
        for(int i = 0; i < numQueries; i++){
            freeDatabase(databases[i]);
        }
        
        //printf("End\n");
    }

    double end_time = MPI_Wtime(); 
    printf("Total program execution time: %f seconds\n", end_time - start_time);

    MPI_Barrier(MPI_COMM_WORLD);

    //printf("End Program\n");

    MPI_Finalize();

    return 0;
}