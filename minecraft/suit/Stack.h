#ifndef _STACK_H
#define _STACK_H
#include "Includes.h"
#include "Graph.h"

struct Stack {
    int top;
    int capacity;
    struct Node** array;
};

struct Stack* createStack(int capacity);

int isStackEmpty(struct Stack* stack);
int isStackFull(struct Stack* stack);

void push(struct Stack* stack, struct Node* node);
struct Node* pop(struct Stack* stack);
struct Node* peek(struct Stack* stack);

void freeStack(struct Stack* stack);
/*Just reverses the stack. Useful for current direction of edges.*/
struct Stack* reverseStack(struct Stack* original);

//Needed for IDA* also useful in general
int find(struct Stack* stack, struct Node* node);

#endif