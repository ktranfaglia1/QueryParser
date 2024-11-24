#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structStack.h"



void initStructStack(structStack* stack) {
    stack->top = NULL;
}

void structPush(structStack* stack, void* value, size_t size) {
    structStackNode* newNode = (structStackNode*)malloc(sizeof(structStackNode));
    if (!newNode) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }

    newNode->data = malloc(size);  
    if (!newNode->data) {
        fprintf(stderr, "Memory allocation for data failed!\n");
        exit(1);
    }

    memcpy(newNode->data, value, size);

    newNode->next = stack->top; 
    stack->top = newNode;
}

void* structPop(structStack* stack) {
    if (stack->top == NULL) {
        fprintf(stderr, "Stack underflow: The stack is empty!\n");
        exit(1);
    }

    structStackNode* temp = stack->top;
    void* value = temp->data; 
    stack->top = stack->top->next;
    free(temp);  
    return value;  
}

void* structPeek(const structStack* stack) {
    if (stack->top == NULL) {
        fprintf(stderr, "Stack is empty!\n");
        exit(1);
    }
    return stack->top->data; 
}

int isStructEmpty(const structStack* stack) {
    return stack->top == NULL;
}

void destroyStructStack(structStack* stack) {
    while (!isStructEmpty(stack)) {
        void* data = structPop(stack);
        free(data); 
    }
}
