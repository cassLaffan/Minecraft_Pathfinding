#ifndef _RBFS_H
#define _RBFS_H
#include "Includes.h"
#include "Graph.h"
#include "Stack.h"
#include "Octree.h"
#include "Queue.h"
#include "Heuristic.h"

/*
* Finds the shortest path to an enterence given the current player's starting point.
*/

float RBFSHelper(struct Graph* graph, struct Node* node, float limit);

struct Stack* RBFS(struct Graph* graph);

#endif