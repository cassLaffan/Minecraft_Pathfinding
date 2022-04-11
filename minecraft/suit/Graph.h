#ifndef _GRAPH_H
#define _GRAPH_H
#include <time.h>
#include "Includes.h"
#define N 10000

/*
* The way this graph is structured is that each node has x,y,z coordinates.
* Graph is dynamic because lord only knows when the person will stop walking and turn around.
*/
struct Node {
    //ID for quick index lookup
    int ID;
    int userID;
    int sequenceID;
    //The adjacent nodes
    struct Node** adjacencyArray;
    //Node pathway, important for Dijkstra
    struct Node* previous;
    float currentDistance;
    float priority;
    //Number of adjacent nodes
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
    int numUsers;
};

/*
* Creates and returns an empty graph.
*/
struct Graph* createGraph();

/*
* Creates and returns a new node given locational information and the player ID.
*/
struct Node* createNode(int ID, int userID, int sequenceID, float x, float y, float z);

/*
* Returns the distance between two nodes.
*/
float distance(struct Node* first, struct Node* second);

/*
* Adds a node to an existing graph.
*/
void addNode(struct Graph* graph, struct Node* node);


/*
* Adds the stuff in another user's graph to the current graph without
* messing with any of the existing adjacencies
*/
void addOtherGraph(struct Graph* current, struct Graph* other);

/*
* Checks if a node is already in the adjacency array. Returns 1 or 0.
*/
int inAdjacencies(struct Node* first, struct Node* second);

/*
* Prints the graph to the terminal.
*/
void printGraph(struct Graph* graph);


#endif