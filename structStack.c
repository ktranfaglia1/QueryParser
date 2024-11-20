#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structstack.h"

typedef struct StackNode {
    void* data;  
    struct StackNode* next;
} StackNode;

typedef struct {
    StackNode* top;
} Stack;

void initStack(Stack* stack) {
    stack->top = NULL;
}

void push(Stack* stack, void* value, size_t size) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
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

void* pop(Stack* stack) {
    if (stack->top == NULL) {
        fprintf(stderr, "Stack underflow: The stack is empty!\n");
        exit(1);
    }

    StackNode* temp = stack->top;
    void* value = temp->data; 
    stack->top = stack->top->next;
    free(temp);  
    return value;  
}

void* peek(const Stack* stack) {
    if (stack->top == NULL) {
        fprintf(stderr, "Stack is empty!\n");
        exit(1);
    }
    return stack->top->data; 
}

int isEmpty(const Stack* stack) {
    return stack->top == NULL;
}

void destroyStack(Stack* stack) {
    while (!isEmpty(stack)) {
        void* data = pop(stack);
        free(data); 
    }
}
