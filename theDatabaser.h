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

#endif