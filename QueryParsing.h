#ifndef Query_Parser_H
#define Query_Parser_H

#include "searchUtility.h"
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


CarContainer* callOperations(CarContainer* database, opTuple* inFixOperations);
ComparisonOperation opToEnum(char* opString);
ComparisonObject strToObject(char* opString);
void* objectToDataType(char* objectString, ComparisonObject compareObject);


#endif