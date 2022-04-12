#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "Queue.h"
#include "Graph.h"

/*
* My pride and joy has gone from a beautiful circular queue to a
* priority queue. Lesson learned at 1:00 in the morning: these two
* data structures aren't compatible with sanity.
*/

struct Queue* createQueue(int capacity) {
    struct Queue* queue = malloc(sizeof(struct Queue*));

    queue->capacity = capacity;
    queue->array = malloc(queue->capacity * sizeof(struct Node*));
    queue->size = 0;

    return queue;
}

int isEmpty(struct Queue* queue) {
    return queue->size == 0;
}

int isFull(struct Queue* queue) {
    return (queue->size == queue->capacity);
}

//p (for priority) = a combination of closest node and heuristic, which is how close the
//node is to the exit
void enqueue(struct Queue* queue, struct Node* node, float p) {
    if (!isFull(queue)) {
        node->f = p;
        queue->array[queue->size] = node;
        queue->size++;
    }
}

struct Node* dequeue(struct Queue* queue) {
    struct Node* node;
    if (!isEmpty(queue)) {

        int max = 0;

        for (int i = 0; i < queue->size; i++) {
            if (queue->array[max]->f < queue->array[max]->f) {
                max = i;
            }
        }

        node = queue->array[max];

        for (int i = max; i < queue->size - 1; i++) {
            queue->array[i] = queue->array[i + 1];
        }

        queue->array[queue->size - 1] = queue->array[queue->size];

        queue->size--;
    }
    else {
        node = NULL;
    }
    return node;
}


void freeQueue(struct Queue* queue) {
    free(queue->array);
    free(queue);
}
