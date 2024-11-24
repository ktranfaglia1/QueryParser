//Timothy McKirgan
//11/18/2024
//COSC 420


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stringStack.h"
#include "parse.h"


char** getArgStrings(int argc, char** argv) {
    //Initialize the count storing the size of query for Select, From, and Where keywords
    int selectSize = 0;
    int fromSize = 0;
    int whereSize = 0;
    //Initialize variables that store information on string parsing progress. progress == 1 if parsing select,  progress == 2 if parsing from, progress == 3 if parsing where. first == 1 if this is the first word following any of the query keywords.
    int progress = 0;
    int first = 0;
    for (int i = 1; i < argc; i++) {
        //If we have reached the select keyword, increment progress and set first to true because we will reach first word of select portion of query
        if (!strcmp("SELECT", argv[i])) {
            progress++;
            first = 1;
        }
        //If we have reached the from keyword, increment progress and set first to true because we will reach first word of from portion of query
        else if (!strcmp("FROM", argv[i])) {
            progress++;
            first = 1;
        }
        //If we have reached the where keyword, increment progress and set first to true because we will reach first word of where portion of query
        else if (!strcmp("WHERE", argv[i])) {
            progress++;
            first = 1;
        }
        //Otherwise check progress to see which query string length this word's size will be added onto. If this is the first word of the query section, only add the word. Otherwise add the word + one more for the space.
        else {
            if (progress == 1) {
                if (first) {
                    first = 0;
                }
                else {
                    selectSize++;
                }
                selectSize += strlen(argv[i]);
            }
            else if (progress == 2) {
                if (first) {
                    first = 0;
                }
                else {
                    fromSize++;
                }
                fromSize += strlen(argv[i]);
            }
            else if (progress == 3) {
                if (first) {
                    first = 0;
                }
                else {
                    whereSize++;
                }
                whereSize += strlen(argv[i]);
            }
        }
    }
    
    //Initialize strings that will store each portion of the query.
    char* selectString = (char*)malloc(sizeof(char) * selectSize + 1);
    char* fromString = (char*)malloc(sizeof(char) * fromSize + 1);
    char* whereString = (char*)malloc(sizeof(char) * whereSize + 1);
    selectString[selectSize] = '\0';
    fromString[fromSize] = '\0';
    whereString[whereSize] = '\0';
    
    //Reset progress and first variables to prepare for another iteration over the argv array.
    progress = 0;
    first = 0;
    for (int i = 1; i < argc; i++) {
        //If we have reached the select keyword, increment progress and set first to true because we will reach first word of select portion of query
        if (!strcmp("SELECT", argv[i])) {
            progress++;
            first = 1;
        }
        //If we have reached the from keyword, increment progress and set first to true because we will reach first word of from portion of query
        else if (!strcmp("FROM", argv[i])) {
            progress++;
            first = 1;
        }
        //If we have reached the where keyword, increment progress and set first to true because we will reach first word of where portion of query
        else if (!strcmp("WHERE", argv[i])) {
            progress++;
            first = 1;
        }
        //Otherwise check progress to see which query string this word size will be appended onto. If this is the first word of the query section, only add the word. Otherwise add the word + an empty space character.
        else {
            if (progress == 1) {
                if (first) {
                    first = 0;
                }
                else {
                    strcat(selectString, " ");
                }
                strcat(selectString, argv[i]);
            }
            else if (progress == 2) {
                if (first) {
                    first = 0;
                }
                else {
                    strcat(fromString, " ");
                }
                strcat(fromString, argv[i]);
            }
            else if (progress == 3) {
                if (first) {
                    first = 0;
                }
                else {
                    strcat(whereString, " ");
                }
                strcat(whereString, argv[i]);
            }
        }
    }
    //Initialize array to contain the three portions of the query and load those portions onto the array
    char** inputStrings = (char**)malloc(sizeof(char*) * 3);
    inputStrings[0] = selectString;
    inputStrings[1] = fromString;
    inputStrings[2] = whereString;
    //Return all three arrays stored in the overall array
    return inputStrings;
}

char** getCSVs(char* inputString) {
    //Initialize size of inputString
    int size = strlen(inputString);
    int count = 1;
    //Count the number of commas + 1 to indicate the number of elements in the CSV
    for (int i = 0; i < size; i++) {
        if (inputString[i] == ',') {
            count++;
        }
    }
    //Initialize a size array to indicate the number of non-space characters between each comma.
    int sizeArray[count];
    for (int i = 0; i < count; i++) {
        sizeArray[i] = 0;
    }
    int pos = 0;
    //Iterate over the inputString. If the character is not a space, increment the current position in the sizeArray, if it is a comma, increment pos as the program is now counting the number of characters in the next comma seperated value.
    for (int i = 0; i < size; i++) {
        if (inputString[i] == ',') {
            pos++;
        }
        else if (inputString[i] != ' ') {
            sizeArray[pos]++;
        }
    }
    
    //Initialize the number of digits in the count.
    int digitNum = 1;
    //Set a float digitTest equivalent to count to test how many characters the number converted to a string should have.
    float digitTest = count;
    //While digitTest >= 10, divide by 10 and increment digitNum.
    while (digitTest >= 10) {
        digitTest = digitTest / 10;
        digitNum++;
    }
    //Initialize a size string that has the number of characters as the size does digts.
    char* sizeString = (char*)malloc(sizeof(char) * (digitNum + 1));
    sizeString[digitNum] == '\0';
    //Copy contents of count + 1 into sizeString as a string.
    sprintf(sizeString, "%d", count + 1);
    //Initialize the CSV array with count + 1 values to store the size of the array as a string at index 0.
    char** csv = (char**)malloc(sizeof(char*) * (count + 1));
    //Set the first element of csv equivalent to the size in string form
    csv[0] = sizeString;
    //Allocate memory for the rest of the elements.
    for (int i = 1; i < count + 1; i++) {
        csv[i] = (char*)malloc(sizeof(char) * (sizeArray[i - 1] + 1));
    }
    //Concatenate the characters of each CSV to the appropriate index.
    pos = 1;
    int pospos = 0;
    for (int i = 0; i < size; i++) {
        if (i == size - 1) {
            csv[pos][pospos] = inputString[i];
            pospos++;
            csv[pos][pospos] = '\0';
        }
        else if (inputString[i] == ',') {
            csv[pos][pospos] = '\0';
            pos++;
            pospos = 0;
        }
        else if (inputString[i] != ' ') {
            csv[pos][pospos] = inputString[i];
            pospos++;
        }
    }
    return csv;
}

char** getOpers(char* inputString) {
    int size = strlen(inputString);
    int count = 0;
    int onOpp = 0;
    for (int i = 0; i < size; i++) {
        if (inputString[i] == ';') {
            if (onOpp) {
                count++;
                onOpp = 0;
            }
            break;
        }
        else if (inputString[i] == ' ') {
            if (onOpp) {
                count++;
                onOpp = 0;
            }
        }
        else if (inputString[i] == '(' || inputString[i] == ')') {
            count++;
            if (onOpp) {
                count++;
                onOpp = 0;
            }
        }
        else {
            if (!onOpp) {
                onOpp = 1;
            }
            if (i == size - 1) {
                if (onOpp) {
                    count++;
                    onOpp = 0;
                }
            }
        }
    }
    
    int sizeArray[count];
    for (int i = 0; i < count; i++) {
        sizeArray[i] = 0;
    }
    int pos = 0;
    for (int i = 0; i < size; i++) {
        if (inputString[i] == ';') {
            break;
        }
        else if (inputString[i] == ' ') {
            if (onOpp) {
                pos++;
                onOpp = 0;
            }
        }
        else if (inputString[i] == '(' || inputString[i] == ')') {
            if (onOpp) {
                pos++;
                onOpp = 0;
            }
            sizeArray[pos] = 1;
            pos++;
        }
        else {
            if (!onOpp) {
                onOpp = 1;
            }
            sizeArray[pos]++;
        }
    }
    
    int digitNum = 1;
    float digitTest = count;
    while (digitTest >= 10) {
        digitTest = digitTest / 10;
        digitNum++;
    }
    char* sizeString = (char*)malloc(sizeof(char) * (digitNum + 1));
    sizeString[digitNum] == '\0';
    sprintf(sizeString, "%d", count + 1);
    char** opp = (char**)malloc(sizeof(char*) * (count + 1));
    opp[0] = sizeString;
    //Allocate memory for the rest of the elements.
    for (int i = 1; i < count + 1; i++) {
        opp[i] = (char*)malloc(sizeof(char) * (sizeArray[i - 1] + 1));
    }
    pos = 1;
    int pospos = 0;
    for (int i = 0; i < size; i++) {
        if (inputString[i] == ';') {
            if (onOpp) {
                opp[pos][pospos] = '\0';
                onOpp = 0;
            }
            break;
        }
        else if (inputString[i] == ' ') {
            if (onOpp) {
                opp[pos][pospos] = '\0';
                pos++;
                onOpp = 0;
                pospos = 0;
            }
        }
        else if (inputString[i] == '(' || inputString[i] == ')') {
            if (onOpp) {
                opp[pos][pospos] = '\0';
                pos++;
                onOpp = 0;
            }
            opp[pos][0] = inputString[i];
            pos++;
            pospos = 0;
        }
        else {
            if (!onOpp) {
                onOpp = 1;
            }
            opp[pos][pospos] = inputString[i];
            pospos++;
        }
    }
    return opp;
}


char*** getParams(int argc, char** argv) {
    char*** parameters = (char***)malloc(sizeof(char**) * 3);
    char** inputStrings = getArgStrings(argc, argv);
    parameters[0] = getCSVs(inputStrings[0]);
    parameters[1] = getCSVs(inputStrings[1]);
    parameters[2] = getOpers(inputStrings[2]);
    free(inputStrings[0]);
    free(inputStrings[1]);
    free(inputStrings[2]);
    free(inputStrings);
    return parameters;
}

void freeParams(char*** par) {
    for (int i = 0; i < 3; i++) {
        int size = atoi(par[i][0]);
        for (int f = 0; f < size; f++) {
            free(par[i][f]);
        }
        free(par[i]);
    }
    free(par);
}

opTuple getTuple(char* element) {
    opTuple instruction;
    if (!strcmp("AND", element)) {
        instruction.dataType = element;
        char* K = (char*)malloc(sizeof(char) * 2);
        K[0] = 'K';
        K[1] = '\0';
        instruction.object = K;
        char* Y = (char*)malloc(sizeof(char) * 2);
        Y[0] = 'Y';
        Y[1] = '\0';
        instruction.condition = Y;
    }
    else if (!strcmp("OR", element)) {
        instruction.dataType = element;
        char* L = (char*)malloc(sizeof(char) * 2);
        L[0] = 'L';
        L[1] = '\0';
        instruction.object = L;
        char* E = (char*)malloc(sizeof(char) * 2);
        E[0] = 'E';
        E[1] = '\0';
        instruction.condition = E;
    }
    else {
        int size = strlen(element);
        for (int i = 0; i < size - 1; i++) {
            if ((element[i] == '>' || element[i] == '<' || element[i] == '!') && (element[i + 1] == '=')) {
                int dataTypeS = 0;
                int conditionS = i;
                int objectS = i + 2;
                char* data = (char*)malloc(sizeof(char) * (conditionS - dataTypeS + 1));
                data[conditionS - dataTypeS] = '\0';
                char* con = (char*)malloc(sizeof(char) * (objectS - conditionS + 1));
                con[objectS - conditionS] = '\0';
                char* ob = (char*)malloc(sizeof(char) * (size - objectS + 1));
                ob[size - objectS] = '\0';
                int dataPos = 0;
                int conPos = 0;
                int obPos = 0;
                for (int f = 0; f < size; f++) {
                    if (f < conditionS) {
                        data[dataPos++] = element[f];
                    }
                    else if (f < objectS) {
                        con[conPos++] = element[f];
                    }
                    else if (f < size) {
                        ob[obPos++] = element[f];
                    }
                }
                instruction.dataType = data;
                instruction.object = ob;
                instruction.condition = con;
                break;
            }
            else if (element[i] == '>' || element[i] == '<' || element[i] == '=') {
                int dataTypeS = 0;
                int conditionS = i;
                int objectS = i + 1;
                char* data = (char*)malloc(sizeof(char) * (conditionS - dataTypeS + 1));
                data[conditionS - dataTypeS] = '\0';
                char* con = (char*)malloc(sizeof(char) * (objectS - conditionS + 1));
                con[objectS - conditionS] = '\0';
                char* ob = (char*)malloc(sizeof(char) * (size - objectS + 1));
                ob[size - objectS] = '\0';
                int dataPos = 0;
                int conPos = 0;
                int obPos = 0;
                for (int f = 0; f < size; f++) {
                    if (f < conditionS) {
                        data[dataPos++] = element[f];
                    }
                    else if (f < objectS) {
                        con[conPos++] = element[f];
                    }
                    else if (f < size) {
                        ob[obPos++] = element[f];
                    }
                }
                instruction.dataType = data;
                instruction.object = ob;
                instruction.condition = con;
                break;
            }
        }
    }
    return instruction;
}

opTuple* where(char** wPar) {
    Stack instructStruct;
    Stack* instruct = &instructStruct;
    initStack(instruct);
    int count = 0;
    for (int i = 1; i < atoi(wPar[0]); i++) {
        if (strcmp("(", wPar[i]) && strcmp(")", wPar[i])) {
            count++;
        }
    }
    opTuple* dir = (opTuple*)malloc(sizeof(opTuple) * count + 1);
    int digitNum = 1;
    float digitTest = count;
    while (digitTest >= 10) {
        digitTest = digitTest / 10;
        digitNum++;
    }
    char* sizeString = (char*)malloc(sizeof(char) * (digitNum + 1));
    sizeString[digitNum] == '\0';
    sprintf(sizeString, "%d", count + 1);
    dir[0].dataType = sizeString;
    char* T = (char*)malloc(sizeof(char) * 2);
    T[0] = 'T';
    T[1] = '\0';
    dir[0].object = T;
    char* M = (char*)malloc(sizeof(char) * 2);
    M[0] = 'M';
    M[1] = '\0';
    dir[0].condition = M;
    int dirPos = 1;
    int inParen = 0;
    for (int i = 1; i < atoi(wPar[0]); i++) {
        if (!strcmp("AND", wPar[i]) || !strcmp("OR", wPar[i])) {
            if (isEmpty(instruct) || inParen > 0) {
                push(instruct, wPar[i]);
            }
            else {
                while(!isEmpty(instruct)) {
                    char* topStr = pop(instruct);
                    dir[dirPos] = getTuple(topStr);
                    dirPos++;
                }
                push(instruct, wPar[i]);
            }
        }
        else if (!strcmp("(", wPar[i])) {
            push(instruct, wPar[i]);
            inParen++;
        }
        else if (!strcmp(")", wPar[i])) {
            int found = 0;
            while (!found) {
                char* topStr = pop(instruct);
                if (!strcmp("(", topStr)) {
                    found = 1;
                }
                else {
                    dir[dirPos] = getTuple(topStr);
                    dirPos++;
                }
            }
            inParen--;
        }
        else {
            dir[dirPos] = getTuple(wPar[i]);
            dirPos++;
        }
    }
    while(!isEmpty(instruct)) {
        char* topStr = pop(instruct);
        dir[dirPos] = getTuple(topStr);
        dirPos++;
    }
    destroyStack(instruct);
    return dir;
}
