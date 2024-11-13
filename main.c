#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**
 * @brief is the function which takes the command line inputs and turns them into 1 string for parsing
 * 
 * @param argc This is the number of strings being taken in to concatenate
 * @param argv This is the strings being concatenated together
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

int main(int argc, char** argv)
{
    char* queryString = concatInput(argc, argv);
}