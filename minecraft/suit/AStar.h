#ifndef _ASTAR_H
#define _ASTAR_H
#include "Includes.h"
#include "Graph.h"
#include "Stack.h"
#include "Octree.h"
#include "Queue.h"
#include "Heuristic.h"

/*
* Finds the shortest path to an enterence given the current player's starting point.
*/
struct Stack* aStarRecreation(struct Graph* graph, int weight);

#endif