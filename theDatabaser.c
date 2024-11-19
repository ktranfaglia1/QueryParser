#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Car {
	int ID;
	char* Model;
	int YearMake;
	char* Color;
	int Price;
	char* Dealer;
} Car;

typedef struct CarContainer {
	int size;#define MAXSTR 100
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

void printDatabase(CarContainer* container) {
    for (int i = 0; i < container->size; i++) {
        printf("%d,%s,%d,%s,%d,%s\n",
            container->array[i].ID, 
            container->array[i].Model, 
            container->array[i].YearMake, 
            container->array[i].Color, 
            container->array[i].Price, 
            container->array[i].Dealer);
    }
}

CarContainer* createDatabase() {
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
