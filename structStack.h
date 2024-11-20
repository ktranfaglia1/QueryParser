#ifndef STRUCTSTACK_H
#define STRUCTSTACK_H

typedef struct StackNode {
    char* data;          
    struct StackNode* next; 
} StackNode;

typedef struct {
    StackNode* top; 
} Stack;

void initStack(Stack* stack);
void push(Stack* stack, void* value, size_t size);
void* pop(Stack* stack);
void* peek(const Stack* stack);
int isEmpty(const Stack* stack);
void destroyStack(Stack* stack);

#endif 
