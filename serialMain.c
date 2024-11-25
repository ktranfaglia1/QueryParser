#include "theDatabaser.h"
#include "QueryParsing.h"
#include "parse.h"


int main(int argc, char** argv) {
    
    int numQueries = 0;
    //Turns Parameter into Parsed Array
    //char*** parameters = getParams(argc, argv, &numQueries); 
    char*** parameters = getParams(argc, argv); 

    int i = 0;

    //for(int i = 0; i < numQueries; i++)
    {
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
    }
}