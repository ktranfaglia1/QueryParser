#include <stdio.h>
#include <time.h>
#include "theDatabaser.h"
#include "QueryParsing.h"
#include "parse.h"


int main(int argc, char** argv) {

    clock_t start_time = clock();
    
    int numQueries = 0;
    //Turns Parameter into Parsed Array
    char*** parameters = getParams(argc, argv, &numQueries); 


    for(int i = 0; i < numQueries; i++)
    {
        clock_t query_start_time = clock();

        printf("\nQuery %d\n\n", i);

        //Select Command Runs 
        CarContainer* database = createDatabase(parameters[i * 3 + 1][1]);

        

        //Converts Operations to postFix
        opTuple* postFix = where(parameters[i * 3 + 2]);

        

        //Runs Operations Accordingly
        database = callOperations(database, postFix); 

        //Outputs Database with proper columns
        printDatabase(database, parameters[i * 3]);

        //Frees Memory
        freeDatabase(database);

        clock_t query_end_time = clock();

        double query_duration = (double)(query_end_time - query_start_time) / CLOCKS_PER_SEC;
        printf("Query took %f seconds\n", query_duration);
    }

    clock_t end_time = clock();
    double total_duration = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("\nTotal runtime: %f seconds\n", total_duration);

    return 0;
}