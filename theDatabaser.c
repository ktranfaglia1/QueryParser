#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "theDatabaser.h"
#include "QueryParsing.h"



void freeDatabase(CarContainer* container) {

    for (int i = 0; i < container->size; i++) {
        free(container->array[i].Model);
        free(container->array[i].Color);
        free(container->array[i].Dealer);
    }
    
    free(container->array);
    free(container);
}

void printDatabase(CarContainer* container, char** params) {
    for (int i = 0; i < container->size; i++) {
        for(int pIndex = 1; pIndex < atoi(params[0]); pIndex++)
        {
            switch (strToObject(params[pIndex]))
            {
            case ID:
                printf("%d,", container->array[i].ID);
                break;
            case MODEL:
                printf("%s,", container->array[i].Model);
                break;
            case COLOR:
                printf("%s,",container->array[i].Color);
                break;
            case MAKE:
                printf("%d,",container->array[i].YearMake);
                break;
            case PRICE:
                printf("%d,",container->array[i].Price);
                break;
            case DEALER:
                printf("%s,",container->array[i].Dealer);
                break;
            default:
                break;
            }
        }
        printf("\n");
    }
}

void shortPrintDatabase(CarContainer* container, char** params) {
    for (int i = 0; i < 10; i++) {
        for(int pIndex = 1; pIndex < atoi(params[0]); pIndex++)
        {
            switch (strToObject(params[pIndex]))
            {
            case ID:
                printf("%d,", container->array[i].ID);
                break;
            case MODEL:
                printf("%s,", container->array[i].Model);
                break;
            case COLOR:
                printf("%s,",container->array[i].Color);
                break;
            case MAKE:
                printf("%d,",container->array[i].YearMake);
                break;
            case PRICE:
                printf("%d,",container->array[i].Price);
                break;
            case DEALER:
                printf("%s,",container->array[i].Dealer);
                break;
            default:
                break;
            }
        }
        printf("\n");
    }
}

CarContainer* createDatabase(char *databaseName) {
    CarContainer* container = (CarContainer*)malloc(sizeof(CarContainer));
    
    //create new database character array to store filename
    char *database = malloc(strlen(databaseName) + 5);
    strcpy(database, databaseName);
    
    // Append ".txt"
    strcat(database, ".txt");
    
	container->size = 0;

	FILE *fp = fopen(database, "r");
	char c;

	if(fp == NULL)
	{
		printf("Error: %s does not exist", database);
		exit(1);
	}

	//Checks for number of lines
	for(c = getc(fp); c != EOF; c = getc(fp)) {
		if(c == '\n')
			container->size+=1;
	}

	//Reset to beginning of file
	rewind(fp);

	//Allocate Space dynamically for car inventory array
	container->array = (Car*)malloc(container->size * sizeof(Car));

    int index = 0;
	while (fscanf(fp, "%d,%m[^,],%d,%m[^,],%d,%m[^,\n]\n", 
            &container->array[index].ID, 
            &container->array[index].Model, 
            &container->array[index].YearMake, 
            &container->array[index].Color, 
            &container->array[index].Price, 
            &container->array[index].Dealer) == 6) {
        
        index++;
    }
    
	fclose(fp);

	return container;
	//freeDatabase(container);
}

Car copyCar(Car currentCar){
    Car newCar;

    newCar = currentCar;
    memcpy(newCar.Dealer, currentCar.Dealer, sizeof(currentCar.Dealer));
    memcpy(newCar.Color, currentCar.Color, sizeof(currentCar.Color));
    memcpy(newCar.Model, currentCar.Model, sizeof(currentCar.Model));

    return newCar;
}

/*
* TEST CASE
* 
int main () {
    char *str = "car_inventory";
    CarContainer* cars = createDatabase(str);
    printDatabase(cars);
    freeDatabase(cars);
    return 0;
}

*/

