#ifndef _EUCLIDEAN_VARIANT_H
#define _EUCLIDEAN_VARIANT_H
#include "../Includes.h"
#include "../Data Structures/Graph.h"

/*
* Computes the h values for all the nodes in the graph.
* This heuristic is the average euclidean distance of a node
* from the goal nodes combined with the number of nodes behind it.
* The reasoning is that I wish to minimize the amount of nodes in a path
* while keeping it close to optimal.
*/
void computeH(struct Graph* graph);


#endif