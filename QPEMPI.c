#include <stdio.h>
#include <time.h>
#include <mpi.h>
#include "theDatabaser.h"
#include "QueryParsing.h"
#include "parse.h"


int main(int argc, char** argv) {

    
    int numQueries = 0;
    //Turns Parameter into Parsed Array
    char*** parameters = getParams(argc, argv, &numQueries); 
    
    CarContainer** databases = (CarContainer**)malloc(sizeof(CarContainer*) * numQueries);
    opTuple** postFix  = (opTuple**)malloc(sizeof(opTuple*) * numQueries);


    //Creates a separate database and post fix for each query

    
    MPI_Init(&argc, &argv);


    for(int i = 0; i < numQueries; i++){
        databases[i] = createDatabase(parameters[i * 3 + 1][1]);

         //Converts Operations to postFix
        postFix[i] = where(parameters[i * 3 + 2]);
    }

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get the rank of the process
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    //int i = 0;
    for(int i = rank; i < numQueries; i+= size)
    {
        

        //printf("\nQuery %d\n\n", i);

        CarContainer* database = (CarContainer*)malloc(sizeof(CarContainer));

        database->size = databases[i]->size * numQueries / size;
        database->array = (Car*)malloc(sizeof(Car) * database->size);

        int counter = 0;
        for(int j = database->size * rank; j < database->size * (rank + 1); j++){
            database->array[counter] = databases[i]->array[j];
            counter++;
        }

        
        
        //Runs Operations Accordingly
        database = callOperations(database, postFix[i]); 

        

        //Outputs Database with proper columns
        printDatabase(database, parameters[i * 3]);

        
        
        //Frees Memory
        freeDatabase(database);
        for(int i = 0; i < numQueries; i++){
            freeDatabase(databases[i]);
        }
        
        

    }

    

    MPI_Barrier(MPI_COMM_WORLD);

    printf("End Program\n");

    MPI_Finalize();

    return 0;
}