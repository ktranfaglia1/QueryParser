#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "stringStack.h"

void initStack(Stack* stack) {
    stack->top = NULL;
}

void push(Stack* stack, const char* value) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    if (!newNode) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }

    newNode->data = strdup(value);  
    if (!newNode->data) {
        fprintf(stderr, "Memory allocation for string failed!\n");
        exit(1);
    }

    newNode->next = stack->top; 
    stack->top = newNode;      
}

char* pop(Stack* stack) {
    if (isEmpty(stack)) {
        fprintf(stderr, "Stack underflow: The stack is empty!\n");
        exit(1);
    }

    StackNode* temp = stack->top;
    char* value = temp->data; 
    stack->top = stack->top->next; 
    free(temp); 
    return value; 
}

//Look at the top string of the stack
char* peek(const Stack* stack) {
    if (isEmpty(stack)) {
        fprintf(stderr, "Stack is empty!\n");
        exit(1);
    }
    return stack->top->data; 
}

int isEmpty(const Stack* stack) {
    return stack->top == NULL;
}

//Destroy the stack and free memory for it
void destroyStack(Stack* stack) {
    while (!isEmpty(stack)) {
        char* data = pop(stack); 
        free(data); 
    }
}
