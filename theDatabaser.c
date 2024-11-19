#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTR 100

typedef struct Car {
	int ID;
	char* Model;
	int YearMake;
	char* Color;
	int Price;
	char* Dealer;
} Car;

typedef struct CarContainer {
	int size;
	Car *array;
} CarContainer;

void freeDatabase(CarContainer* container) {
    for (int i = 0; i < container->size; i++) {
        free(container->array[i].Model);
        free(container->array[i].Color);
        free(container->array[i].Dealer);
    }
    free(container->array);
    free(container);
}

int main () {
//CarContainer* createDatabase() {
    CarContainer* container = (CarContainer*)malloc(sizeof(CarContainer));

	container->size = 0;

    //Enter Database file name
	char *database = "car_inventory.txt";	
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
	while (fscanf(fp, "%d,%ms,%d,%ms,%d,%ms\n", 
            &container->array[index].ID, 
            &container->array[index].Model, 
            &container->array[index].YearMake, 
            &container->array[index].Color, 
            &container->array[index].Price, 
            &container->array[index].Dealer) == 6) {
        index++;
    }

	fclose(fp);
	freeDatabase(container);

	return 0;
}
