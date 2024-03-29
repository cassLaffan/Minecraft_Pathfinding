#ifndef _IDASTAR_H
#define _IDASTAR_H
#include "../Includes.h"
#include "../Data Structures/Graph.h"
#include "../Data Structures/Stack.h"
#include "../Data Structures/AdjacencySearch.h"
#include "../Data Structures/Queue.h"
#include "EuclideanHeuristic.h"
#include "ZeroHeuristic.h"
#include "ManhattanHeuristic.h"

// Graph is only included to tally expansions
void searchForPlan(struct Graph* graph, struct Stack* path, struct Node* node);
float searchHelper(struct Graph* graph, struct Stack* path, float g, float bound);

/*
* Finds the shortest path to an enterence given the current player's starting point.
*/
struct Stack* IDAStar(struct Graph* graph);

#endif
