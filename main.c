#include "theDatabaser.c"
#include "QueryParsing.c"


int main(int argc, char** argv) {
    char* queryString = concatInput(argc, argv);
    char*** parameters = getParams(); 


    CarContainer* database = createDatabase(parameters[1][1]);

 
    opTuple* infix = where(parameters[2]);

    database = callOperations(database, infix);

    select(database, parameters[0]);
    freeDatabase(database);
}