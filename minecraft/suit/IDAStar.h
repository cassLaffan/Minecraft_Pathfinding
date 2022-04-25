#ifndef _IDASTAR_H
#define _IDASTAR_H
#include "Includes.h"
#include "Graph.h"
#include "Stack.h"
#include "Octree.h"
#include "Queue.h"
#include "Heuristic.h"

void searchForPlan(struct Stack* path, struct Node* node);
float searchHelper(struct Stack* path, float g, float bound);

/*
* Finds the shortest path to an enterence given the current player's starting point.
*/
struct Stack* IDAStar(struct Graph* graph);

#endif