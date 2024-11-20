#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "searchUtility.c"
#include "structStack.h"


/**
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
 * @brief is the function which takes the command line inputs and turns them into 1 string for parsing
 * 
 * @param argc {int} This is the number of strings being taken in to concatenate
 * @param argv {char**} This is the strings being concatenated together
 * 
 * @return concatString the combined String
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
            //printf("%c", concatString[count]);
        }
        concatString[count] = ' ';
        count++;
    }

    concatString[count-1] = '\0';
    return concatString;
}


/**
 * @brief Loops through array of InFix Operations to be called for Where Command
 * 
 * @param database {CarContainer*} Complete Database includes everything inside of the file
 * @param inFixOperations {opTuple*} contains array of operations in InFix Notation
 */
CarContainer* callOperations(CarContainer* database, opTuple* inFixOperations){
    Stack* dataStack;
    initStack(dataStack);


    for(int i = 1; i < atoi(inFixOperations[0].dataType);i++){
        if(strcmp(inFixOperations[i].dataType, 'AND')){
            CarContainer* intersectData = intersect_arrays(pop(dataStack), pop(dataStack));
            push(dataStack, intersectData, sizeof(intersectData));
        }
        else if(strcmp(inFixOperations[i].dataType, 'OR')){
            CarContainer* unionData =  union_arrays(pop(dataStack), pop(dataStack));
            push(dataStack, unionData, sizeof(unionData));
        } else{
            CarContainer* newData =  find_all(database, inFixOperations[i].dataType, inFixOperations[i].condition, inFixOperations[i].object);
            push(dataStack, newData, sizeof(newData)); 
        }
    }

    CarContainer* finalResult = pop(dataStack);
    destroyStack(dataStack);

    return finalResult;
}
