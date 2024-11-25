#include "theDatabaser.h"
#include "QueryParsing.h"
#include "parse.h"


int main(int argc, char** argv) {
    
    int numQueries = 0;
    //Turns Parameter into Parsed Array
    char*** parameters = getParams(argc, argv, &numQueries); 
    //char*** parameters = getParams(argc, argv); 

    printf("NUM QUERIES %d\n", numQueries);

    for(int i = 0; i < numQueries; i++)
    {
        //Select Command Runs 
        CarContainer* database = createDatabase(parameters[i * 3 + 1][1]);

        

        //Converts Operations to postFix
        opTuple* postFix = where(parameters[i * 3 + 2]);
        for (int f = 1; f < atoi(postFix[0].dataType); f++) {
            printf("(");
            printf("%s, ", postFix[f].dataType);
            printf("%s, ", postFix[f].object);
            printf("%s)\n", postFix[f].condition);
        }
        printf("\n");

        

        //Runs Operations Accordingly
        database = callOperations(database, postFix); 

        //Outputs Database with proper columns
        printDatabase(database, parameters[i * 3]);

        //Frees Memory
        freeDatabase(database);
    }
}