#include "Queue.h"

struct Queue* createQueue(int capacity) {
    struct Queue* queue = malloc(sizeof(struct Queue));

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

void enqueue(struct Queue* queue, struct Node* node, float p) {
    if (!isFull(queue)) {
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
