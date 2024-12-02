#ifndef THE_DATABASER_H 
#define THE_DATABASER_H 

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
	int capacity;
	Car *array;
} CarContainer;


//Function Prototypes
void freeDatabase(CarContainer* container);
void printDatabase(CarContainer* container, char** selectCategories);
void shortPrintDatabase(CarContainer* container);
CarContainer* createDatabase(char *databaseName);

Car copyCar(Car);

#endif