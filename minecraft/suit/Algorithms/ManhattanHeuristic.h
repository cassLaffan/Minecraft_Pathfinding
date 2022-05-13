#ifndef _MANHATTAN_HEURISTIC_H
#define _MANHATTAN_HEURISTIC_H
#include "../Includes.h"
#include "../Data Structures/Graph.h"

/*
* Computes the h values for all the nodes in the graph.
* This heuristic is the average Manhattan distance of a node
* from the goal nodes.
*/
void manComputeH(struct Graph* graph, struct Node* node);


#endif
