#ifndef _SEARCH_H
#define _SEARCH_H
#include "../Includes.h"
#include "Graph.h"
#include "Queue.h"

/*
* Goes through the nodes of the graph to populate adjacency array
*/
void findAdjecencies(struct Graph* graph);


/*
* Initializes the empty queues for RBFS. Works under the assumption findAdjecencies
* has already been run.
*/
void addQueues(struct Graph* graph);

#endif
