#ifndef STRINGSTACK_H
#define STRINGSTACK_H

typedef struct StackNode {
    char* data;          
    struct StackNode* next; 
} StackNode;

typedef struct Stack{
    StackNode* top; 
} Stack;

void initStack(Stack* stack);
void push(Stack* stack, const char* value);
char* pop(Stack* stack);
char* peek(const Stack* stack);
int isEmpty(const Stack* stack);
void destroyStack(Stack* stack);

#endif 
