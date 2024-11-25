#ifndef PARSE_H
#define PARSE_H

#include "QueryParsing.h"

char** getArgStrings(int argc, char** argv, int* size);
char** getCSVs(char* inputString);
char** getOpers(char* inputString) ;
char*** getParams(int argc, char** argv, int* size);
void freeArgStrings(char** string, int size);
void freeParams(char*** par);
opTuple getTuple(char* element);
opTuple* where(char** wPar);

#endif