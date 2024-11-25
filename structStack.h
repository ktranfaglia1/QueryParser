#ifndef STRUCTSTACK_H
#define STRUCTSTACK_H

typedef struct structStackNode {
    char* data;          
    struct structStackNode* next; 
} structStackNode;

typedef struct structStack{
    structStackNode* top; 
} structStack;

void initStructStack(structStack* stack);
void structPush(structStack* stack, void* value, size_t size);
void* structPop(structStack* stack);
void* structPeek(const structStack* stack);
int isStructEmpty(const structStack* stack);
void destroyStructStack(structStack* stack);

#endif 
