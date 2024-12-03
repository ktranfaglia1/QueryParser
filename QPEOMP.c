#include "theDatabaser.h"
#include "parse.h"
#include <stdio.h>
#include <omp.h>

int main(int argc, char** argv) {

    int numQueries = 0;
    
    // Turns Parameter into Parsed Array
    char*** parameters = getParams(argc, argv, &numQueries); 

    double totalStartTime = omp_get_wtime();  // Start total timer

    for(int i = 0; i < numQueries; i++) {

        printf("\nQuery %d\n\n", i);

        double queryStartTime = omp_get_wtime();  // Start query timer

        // Select Command Runs 
        CarContainer* database = createDatabase(parameters[i * 3 + 1][1]);

        // Converts Operations to postFix
        opTuple* postFix = where(parameters[i * 3 + 2]);

        // Runs Operations Accordingly
        database = callOperations(database, postFix); 

        // Outputs Database with proper columns
        printDatabase(database, parameters[i * 3]);

        // Frees Memory
        freeDatabase(database);

        double queryEndTime = omp_get_wtime();  // End query timer
        printf("Query %d ran in %.6f seconds.\n", i, queryEndTime - queryStartTime);  // Print query time
    }

    double totalEndTime = omp_get_wtime();  // End total timer
    printf("\nTotal run time: %.6f seconds.\n", totalEndTime - totalStartTime);  // Print total time

    return 0;
}
