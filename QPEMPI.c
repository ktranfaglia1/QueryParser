#include "theDatabaser.h"
#include "parse.h"
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {

    
    int numQueries = 0;
    
    // Turns Parameter into Parsed Array
    char*** parameters = getParams(argc, argv, &numQueries); 

    
    MPI_Init(&argc, &argv);

    double totalStartTime = MPI_Wtime();  // Start total timer
    for(int i = 0; i < numQueries; i++) {

        printf("\nQuery %d\n\n", i);

        double queryStartTime = MPI_Wtime();  // Start query timer

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

        double queryEndTime = MPI_Wtime();  // End query timer
        printf("Query %d ran in %.6f seconds.\n", i, queryEndTime - queryStartTime);  // Print query time
    }

    double totalEndTime = MPI_Wtime();  // End total timer
    printf("\nTotal run time: %.6f seconds.\n", totalEndTime - totalStartTime);  // Print total time

    MPI_Finalize();
}
