#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Graph.h"

struct Graph* createGraph() {
    struct Graph* graph = malloc(sizeof(struct Graph));
    graph->starts = malloc(sizeof(struct Node*) * N);;
    graph->used = 0;
    graph->numUsers = 0;
    graph->expansions = 0;
    graph->reExpansions = 0;
    return graph;
}

/*
* These nodes are rather complex and I would be open to breaking them into a set
* of smaller structs contained in a big struct.
*/
struct Node* createNode(int ID, int userID, int sequenceID, float x, float y, float z) {
    struct Node* node = malloc(sizeof(struct Node));
    if (node == NULL) {
        printf("How did you fuck this up? You're just creating a node. All the info is there! Ridiculous.\n");
    }
    else {
        node->ID = ID;
        node->userID = userID;
        // The sequence number which indicates its location in a given user's path.
        // 0 is the start of a user's journey/
        node->sequenceID = sequenceID;
        node->previous = malloc(sizeof(struct Node*));

        node->x = x;
        node->y = y;
        node->z = z;
        node->adjacent = 0;
        //Now populated in the Octree algorithm
        node->adjacencyArray = malloc(sizeof(struct Node*) * N);
        node->isFinish = 0;

        node->f = 0;
        node->g = FLT_MAX;
        // The average Euclidean distance from node to each goal (if there is more than 1)
        node->h = 0;
        node->visited = 0;
    }
    return node;
}



float distance(struct Node* first, struct Node* second) {
    return sqrt(pow(first->x - second->x, 2) + pow(first->y - second->y, 2) + pow(first->z - second->z, 2));
}

/*
* It occured to me that it would make my life so much easier if
* I reversed the direction of the node edges. Think about it:
* Having them go backwards means I don't have to search from every
* starting node. Instead, by virtue of how A* works, it'll find the 
* closest exit node.
*/
void addNode(struct Graph* graph, struct Node* node){
    if (graph->used == 0){
        graph->nodes[0] = node;
        graph->used++;
    }
    else{
        // Now the ordering is node_n -> node_{n-1} instead of node_{n-1} -> node_n
        graph->nodes[graph->used] = node;
        graph->nodes[graph->used]->adjacencyArray[0] = graph->nodes[graph->used - 1]; 
        graph->nodes[graph->used]->adjacent++;
        // previous is necessary because it allows the algorithm to walk through the resulting path. I wonder
        // if this eliminates the stack? We will see.
        graph->nodes[graph->used - 1]->previous = graph->nodes[graph->used];
        //g cost to get to this node, will be updated as A* does its thing
        graph->nodes[graph->used]->g = distance(graph->nodes[graph->used], graph->nodes[graph->used]->previous) + graph->nodes[graph->used]->g;
        graph->used++;
    }
    // Adds it to the array of starting points if its sequence ID is 0
    if (node->sequenceID == 0) {
        graph->starts[graph->numUsers] = node;
        graph->numUsers++;
    }
}

void addOtherGraph(struct Graph* current, struct Graph* other) {
    for (int i = 0; i < other->used; i++) {
        current->nodes[current->used + i] = other->nodes[i];
    }
    current->used += other->used;
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

void printGraph(struct Graph* graph) {
    for (int i = 0; i < graph->used; i++) {
        struct Node* node = graph->nodes[i];
        printf("Node: %f, %f, %f, %d, index: %d\n", node->x, node->y, node->z, node->isFinish, node->sequenceID);
        for (int j = 0; j < node->adjacent; j++) {
            printf("Adjacent to: %f, %f, %f, %d\, index: %d \n", node->adjacencyArray[j]->x, node->adjacencyArray[j]->y, node->adjacencyArray[j]->z, node->adjacencyArray[j]->isFinish, node->adjacencyArray[j]->sequenceID);
        }
    }
}
