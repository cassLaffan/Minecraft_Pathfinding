#include <stdio.h>
#include <stdlib.h>
#include "GraphRecreation.h"

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