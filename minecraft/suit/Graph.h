#ifndef _GRAPH_H
#define _GRAPH_H
#include "Includes.h"
#include <time.h>
#define N 10000
/*
* The way this graph is structured is that each node has x,y,z coordinates.
* Graph is dynamic because lord only knows when the person will stop walking and turn around.
*/

struct Node{
    //ID for quick index lookup
    int ID;
    struct Node** adjacencyArray;
    int adjacent;
    //coordinates
    float x;
    float y;
    float z;
    //1 or 0
    int hasObstacle;
    int visited;
};

struct Graph {
    struct Node* nodes[N];
    int used;
};

struct Graph* createGraph();

struct Node* createNode(int id, float x, float y, float z);
void addNode(struct Graph* graph, struct Node* node);

int inAdjacencies(struct Node* first, struct Node* second);

void printGraph(struct Graph* graph);


#endif