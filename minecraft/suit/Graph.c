#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
#include <math.h>

struct Graph* createGraph(){
    struct Graph* graph = malloc(sizeof(struct Graph));
    graph->used = 0;
    return graph;
}

/*
* These nodes are rather complex and I would be open to breaking them into a set
* of smaller structs contained in a big struct. 
*/
struct Node* createNode(int id, float x, float y, float z){
    struct Node* node = malloc(sizeof(struct Node));
    if (node == NULL) {
        printf("Failure lmao\n");
    }
    else {
        node->ID = id; //by default
        node->x = x;
        node->y = y;
        node->z = z;
        node->adjacent = 0;
        node->visited = 0;
        //Now populated in the Octree algorithm
        node->adjacencyArray = malloc(sizeof(struct Node*) * N);
        node->hasObstacle = 0;
    }
    return node;
}

void addNode(struct Graph* graph, struct Node* node){
    if (graph->used == 0){
        graph->nodes[0] = node;
        graph->used++;
    }
    else{
        graph->nodes[graph->used - 1]->adjacencyArray[0] = node; //-1 is because the adjacency array of the previous node is being set
        graph->nodes[graph->used - 1]->adjacent++;
        graph->nodes[graph->used] = node;
        graph->used++;
    }
}

int inAdjacencies(struct Node* first, struct Node* second) {
    int flag = 0;
    for (int i = 0; i < first->adjacent; i++) {
        if (first->adjacencyArray[i] == second) {
            flag = 1;
            break;
        }
    }
    return flag;
}

void printGraph(struct Graph* graph){
    for (int i = 0; i < graph->used; i++){ 
        struct Node* node = graph->nodes[i];
        printf("Node: %f, %f, %f, %d, index: %d\n", node->x, node->y, node->z, node->hasObstacle, node->ID);
        for (int j = 0; j < node->adjacent; j++) {
           printf("Adjacent to: %f, %f, %f, %d\, index: %d \n", node->adjacencyArray[j]->x, node->adjacencyArray[j]->y, node->adjacencyArray[j]->z, node->adjacencyArray[j]->hasObstacle, node->adjacencyArray[j]->ID);
        }
    }
}
