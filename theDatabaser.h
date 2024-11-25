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
	Car *array;
} CarContainer;


//Function Prototypes
void freeDatabase(CarContainer* container);
void printDatabase(CarContainer* container, char** selectCategories);
void shortPrintDatabase(CarContainer* container, char** selectCategories);
CarContainer* createDatabase(char *databaseName);

Car copyCar(Car);

#endif