#ifndef _ASTAR_H
#define _ASTAR_H
#include "Includes.h"
#include "Graph.h"
#include "Stack.h"
#include "Octree.h"
#include "Queue.h"

/*
* Computes the h values for all the nodes in the graph.
*/
void computeH(struct Graph* graph);

/*
* Finds the shortest path to an enterence given the current player's starting point.
*/
struct Stack* aStarRecreation(struct Graph* graph, int weight);

#endif