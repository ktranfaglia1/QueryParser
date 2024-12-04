#include <stdio.h>
#include <time.h>
#include "theDatabaser.h"
#include "QueryParsing.h"
#include "parse.h"


int main(int argc, char** argv) {

    
    int numQueries = 0;
    //Turns Parameter into Parsed Array
    char*** parameters = getParams(argc, argv, &numQueries); 
    
    CarContainer** databases = (CarContainer**)malloc(sizeof(CarContainer*) * numQueries);

    for(int i = 0; i < numQueries; i++){
        databases[i] = createDatabase(parameters[i * 3 + 1][1]);
    }
    


    for(int i = 0; i < numQueries; i++)
    {
        clock_t query_start_time = clock();

        printf("\nQuery %d\n\n", i);

        //Select Command Runs 
        

        

        //Converts Operations to postFix
        opTuple* postFix = where(parameters[i * 3 + 2]);

        

        //Runs Operations Accordingly
        CarContainer* database = callOperations(databases[i], postFix); 

        //Outputs Database with proper columns
        printDatabase(database, parameters[i * 3]);

        //Frees Memory
        freeDatabase(database);

        clock_t query_end_time = clock();

        double query_duration = (double)(query_end_time - query_start_time) / CLOCKS_PER_SEC;

    }


    return 0;
}