#ifndef _RBFS_H
#define _RBFS_H
#include "../Includes.h"
#include "../Data Structures/Graph.h"
#include "../Data Structures/Stack.h"
#include "../Data Structures/Octree.h"
#include "../Data Structures/Queue.h"
#include "EuclideanHeuristic.h"
//#include "ZeroHeuristic.h"
//#include "ManhattanHeuristic.h
//#include "EuclideanVariant.h""

/*
* Finds the shortest path to an enterence given the current player's starting point.
*/

float RBFSHelper(struct Graph* graph, struct Node* node, float limit);

struct Stack* RBFS(struct Graph* graph);

#endif