#ifndef _ASTAR_H
#define _ASTAR_H
#include "../Includes.h"
#include "../Data Structures/Graph.h"
#include "../Data Structures/Stack.h"
#include "../Data Structures/Octree.h"
#include "../Data Structures/Queue.h"
#include "Heuristic.h"

/*
* Finds the shortest path to an enterence given the current player's starting point.
*/
struct Stack* aStarRecreation(struct Graph* graph, int weight);

#endif