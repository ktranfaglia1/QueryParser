#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "searchUtility.c"
#include "structStack.h"


/**  
 * \struct opTuple
 * @brief this is the data structure oriented at holding operations to be executed
 * @param dataType type{char*} This is the column to take from ex. "Year" , "Model" ...
 * @param object type{char*} this is the actual cars information ex. "2016" "Accord" ...
 * @param condition type{char*} specific operations to determine the array ex. >= < =
 */
typedef struct opTuple{
    char* dataType;
    char* object;
    char* condition;
} opTuple;


/**
 * \fn concatInput
 * @brief is the function which takes the command line inputs and turns them into 1 string for parsing
 * 
 * @param argc {int} This is the number of strings being taken in to concatenate
 * @param argv {char**} This is the strings being concatenated together
 * 
 * @return {char*} concatString the combined String
 */
char* concatInput(int argc, char** argv){
    int strSize = 0;

    for(int i = 0; i < argc; i++){
        strSize += strlen(argv[i]);
    }
    strSize += argc - 1;

    char* concatString = (char*)malloc(sizeof(char) * strSize);


    int count = 0;
    for(int i = 1; i < argc; i++){

        for(int j = 0; j < strlen(argv[i]); j++, count++){
            concatString[count] = argv[i][j];
        }
        concatString[count] = ' ';
        count++;
    }

    concatString[count-1] = '\0';
    return concatString;
}


/**
 * \fn callOperations
 * @brief Loops through array of InFix Operations to be called for Where Command
 * 
 * @param database {CarContainer*} Complete Database includes everything inside of the file
 * @param inFixOperations {opTuple*} contains array of operations in InFix Notation
 * 
 * @see find_all(), intersect_arrays(), union_arrays()
 * 
 * @return {CarContainer*} New Reduced Database
 */
CarContainer* callOperations(CarContainer* database, opTuple* inFixOperations){
    Stack actualStack;
    Stack* dataStack = &actualStack;
    initStack(dataStack);


    for(int i = 1; i < atoi(inFixOperations[0].dataType);i++){
        if(strcmp(inFixOperations[i].dataType, 'AND')){

            CarContainer* database1 = pop(dataStack);
            CarContainer* database2 = pop(dataStack);

            CarContainer* intersectData = intersect_arrays(database1, database2);
            push(dataStack, intersectData, sizeof(intersectData));

            freeDatabase(database1);
            freeDatabase(database2);
        }
        else if(strcmp(inFixOperations[i].dataType, 'OR')){
            
            CarContainer* database1 = pop(dataStack);
            CarContainer* database2 = pop(dataStack);

            CarContainer* unionData =  union_arrays(database1, database2);
            push(dataStack, unionData, sizeof(unionData));

            freeDatabase(database1);
            freeDatabase(database2);
        } else{
            CarContainer* newData =  find_all(database, inFixOperations[i].object, opToEnum(inFixOperations[i].condition), strToObject(inFixOperations[i].dataType));
            push(dataStack, newData, sizeof(newData)); 
        }
    }

    CarContainer* finalResult = pop(dataStack);
    destroyStack(dataStack);

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
    } else if (strcmp(opString, "MODEL") == 0){
        finalObject = MODEL;
    }  else if (strcmp(opString, "MAKE") == 0){
        finalObject = MAKE;
    }  else if (strcmp(opString, "COLOR") == 0){
        finalObject = COLOR;
    }  else if (strcmp(opString, "PRICE") == 0){
        finalObject = PRICE;
    }  else{
        finalObject = DEALER;
    } 
    



    return finalObject;
}