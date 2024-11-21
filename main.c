#include "theDatabaser.c"
#include "QueryParsing.c"


int main(int argc, char** argv) {
    
    //Turns Parameter into Parsed Array
    char*** parameters = getParams(); 

    //Select Command Runs 
    CarContainer* database = createDatabase(parameters[1][1]);

    //Converts Operations to postFix
    opTuple* postFix = where(parameters[2]);

    //Runs Operations Accordingly
    database = callOperations(database, postFix);

    //Outputs Database with proper columns
    select(database, parameters[0]);

    //Frees Memory
    freeDatabase(database);
}