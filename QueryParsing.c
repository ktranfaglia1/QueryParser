/**
 * @file QueryParsing.c
 * 
 * @brief All Functions Relating to Handling WHERE PostFix Querys
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "QueryParsing.h"
#include "theDatabaser.h"

/* TODO: Fix memory leaks */


/**
 * \fn concatInput
 * @brief is the function which takes the command line inputs and turns them into 1 string for parsing
 * 
 * @param argc {int} This is the number of strings being taken in to concatenate
 * @param argv {char**} This is the strings being concatenated together
 * 
 * @return {char*} concatString the combined String
 */


/**
 * 
 * \fn callOperations
 * 
 * @file QueryParsing.c
 * QueryParsing.c
 * @brief Loops through array of InFix Operations to be called for Where Command
 * 
 * 
 * 
 * @param database {CarContainer*} Complete Database includes everything inside of the file
 * @param inFixOperations {opTuple*} contains array of operations in InFix Notation
 * 
 * @see find_all(), intersect_arrays(), union_arrays()
 * 
 * @return {CarContainer*} New Reduced Database
 */
CarContainer* callOperations(CarContainer* database, opTuple* inFixOperations){
    structStack actualStack;
    structStack* dataStack = &actualStack;
    initStructStack(dataStack);

    CarContainer* database1;
    CarContainer* database2;

    CarContainer* newData;
    CarContainer* intersectData;
    CarContainer* unionData;
   

   
    
    for(int i = 1; i < atoi(inFixOperations[0].dataType);i++){
        if(strcmp(inFixOperations[i].dataType, "AND") == 0){
            database1 = structPop(dataStack);
            database2 = structPop(dataStack);

            intersectData = intersect_arrays(database1, database2);
            structPush(dataStack, intersectData, sizeof(*intersectData));


            freeDatabase(database1);
            freeDatabase(database2);
        }
        else if(strcmp(inFixOperations[i].dataType, "OR") == 0){
            database1 = structPop(dataStack);
            database2 = structPop(dataStack);

            unionData =  union_arrays(database1, database2);
            structPush(dataStack, unionData, sizeof(*unionData));

            //shortPrintDatabase(unionData);

            freeDatabase(database1);
            freeDatabase(database2);

        } else{
            ComparisonObject dataType = strToObject(inFixOperations[i].dataType);

            //::BREAKS HERE::
            newData =  find_all(database, objectToDataType(inFixOperations[i].object, dataType), opToEnum(inFixOperations[i].condition), dataType);
            structPush(dataStack, newData, sizeof(*newData)); 

            shortPrintDatabase(newData);
        }

    }

    CarContainer* finalResult = structPop(dataStack);
    destroyStructStack(dataStack);

    return finalResult;
}


/**
 * \fn callOperations
 * @brief Converts a string containing the string version of operation to kyles actual operation ENUM
 * 
 * @param opString {char*}  The actual string being Converted
 * 
 * @return {ComparisonOperation} New enum based Operation
 */
ComparisonOperation opToEnum(char* opString){
    ComparisonOperation comparOP;


    if (strcmp(opString, "<=") == 0){
        comparOP = GREATER_EQUAL;
    } else if(strcmp(opString, ">=") == 0){
        comparOP = LESS_EQUAL;
    }else if(strcmp(opString, ">") == 0){
        comparOP = LESS_THAN;
    } else if(strcmp(opString, "<") == 0){
        comparOP = GREATER_THAN;
    } else if(strcmp(opString, "!=") == 0){
        comparOP = NOT_EQUAL_TO;
    } else{
        comparOP = EQUAL_TO;
    }



    return comparOP;
}

/**
 * \fn
 * @brief Converts a string containing the string version of operation to kyles actual operation ENUM
 * 
 * @param opString {char*}  The actual string being Converted
 * 
 * @return {ComparisonObject} New enum based Object
 */
ComparisonObject strToObject(char* opString){
    ComparisonObject finalObject;

    if (strcmp(opString, "ID") == 0){
        finalObject = ID;
    } else if (strcmp(opString, "Model") == 0){
        finalObject = MODEL;
    }  else if (strcmp(opString, "YearMake") == 0){
        finalObject = MAKE;
    }  else if (strcmp(opString, "Color") == 0){
        finalObject = COLOR;
    }  else if (strcmp(opString, "Price") == 0){
        finalObject = PRICE;
    }  else{
        finalObject = DEALER;
    } 
    



    return finalObject;
}

char* removeQuotes(char* objectString){
    int stringSize = strlen(objectString);
    for(int i=0; i < stringSize-1; i++){
        objectString[i] = objectString[i+1];
    }

    
    objectString[stringSize - 2] = '\0';
    return objectString;
}

void* objectToDataType(char* objectString, ComparisonObject compareObject){
    objectString = removeQuotes(objectString);
    if(compareObject == ID || compareObject == PRICE || compareObject == MAKE){
        int* result = (int*)malloc(sizeof(int));
        *result = atoi(objectString);
        return (void*) result;
    }
    else{
        return objectString;
    }
    
}