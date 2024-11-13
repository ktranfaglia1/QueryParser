#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**
 * This is the function which takes the command line inputs and turns them into 1 string for parsing
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

int main(int argc, char** argv)
{
    char* queryString = concatInput(argc, argv);
}