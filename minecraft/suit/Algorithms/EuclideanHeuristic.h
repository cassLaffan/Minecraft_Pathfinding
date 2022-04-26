#ifndef _EUCLIDEAN_HEURISTIC_H
#define _EUCLIDEAN_HEURISTIC_H
#include "../Includes.h"
#include "../Data Structures/Graph.h"

/*
* Computes the h values for all the nodes in the graph.
* This heuristic is the average euclidean distance of a node 
* from the goal nodes.
*/
void computeH(struct Graph* graph);


#endif