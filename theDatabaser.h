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

// Comparison operations
typedef enum {
    GREATER_THAN,
    GREATER_EQUAL,
    LESS_THAN,
    LESS_EQUAL,
    EQUAL_TO,
    NOT_EQUAL_TO,
} ComparisonOperation;

// Comparison objects
typedef enum {
    ID,
    MODEL,
    MAKE,
    COLOR,
    PRICE,
    DEALER,
} ComparisonObject;


//Function Prototypes
void freeDatabase(CarContainer* container);
void printDatabase(CarContainer* container, char** selectCategories);
void shortPrintDatabase(CarContainer* container);
ComparisonObject strToObject(char* opString);
CarContainer* createDatabase(char *databaseName);

Car copyCar(Car);

#endif