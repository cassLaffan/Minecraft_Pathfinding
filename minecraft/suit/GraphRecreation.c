#include <stdio.h>
#include <stdlib.h>
#include "GraphRecreation.h"


/*
* This is the most basic of the path recreation algorithms. 
* It takes the last element in the adjacency array for the
* first node as the next element in the most "efficient" path
* back. This naive approach works for only one player, and always 
* works under the assumption that the absolute latest node that 
* exists in an adjacency array is the correct one. 
* 
* Works as a good sanity check for other algorithms.
*/
struct Stack* simplePathRecreation(struct Graph* graph) {
	findAdjecencies(graph);
	printGraph(graph);
	struct Stack* path = createStack(graph->used);
	//Current node ID
	int currID = graph->nodes[0]->ID;
	//Number of elements in the current node
	int elements = graph->nodes[0]->adjacent;

	do {
		push(path, graph->nodes[currID]);
		elements = graph->nodes[currID]->adjacent - 1;
		if (currID == graph->used - 1) {
			break;
		}
		else {
			currID = graph->nodes[currID]->adjacencyArray[elements]->ID;
		}
	} while (currID <= graph->used - 1);
	
	return path;
}