#ifndef PARSE_H
#define PARSE_H

#include "QueryParsing.h"

char** getArgStrings(int argc, char** argv);
char** getCSVs(char* inputString);
char** getOpers(char* inputString) ;
char*** getParams(int argc, char** argv);
void freeParams(char*** par);
opTuple getTuple(char* element);
opTuple* where(char** wPar);

#endif