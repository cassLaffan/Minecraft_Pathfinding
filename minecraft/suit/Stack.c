#include "Stack.h"

struct Stack* createStack(int capacity) {
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (struct Node**)malloc(stack->capacity * sizeof(struct Node*));
    return stack;
}

int isStackFull(struct Stack* stack) {
    return stack->top == stack->capacity - 1;
}

int isStackEmpty(struct Stack* stack) {
    return stack->top == -2;
}

void push(struct Stack* stack, struct Node* node) {
    if (!isStackFull(stack)) {
        stack->top++;
        stack->array[stack->top] = node;
    }
}

struct Node* pop(struct Stack* stack) {
    return !isStackEmpty(stack) ? stack->array[stack->top--] : NULL;
}

struct Node* peek(struct Stack* stack) {
    return !isStackEmpty(stack) ? stack->array[stack->top] : NULL;
}


void freeStack(struct Stack* stack) {
    free(stack->array);
    free(stack);
}

struct Stack* reverseStack(struct Stack* original) {
    struct Stack* stack = createStack(original->capacity);

    while (!isStackEmpty(original)) {
        struct Node* node = pop(original);
        push(stack, node);
    }
    freeStack(original);
    printf("%d\n", stack->top);
    return stack;
}