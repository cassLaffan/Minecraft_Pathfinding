#include "AdjacencySearch.h"

void findAdjecencies(struct Graph* graph) {
	for (int i = 0; i < graph->used; i++) {
		for (int j = 0; j < graph->used; j++) {
			if (distance(graph->nodes[i], graph->nodes[j]) < 4 && !inAdjacencies(graph->nodes[i], graph->nodes[j]) && graph->nodes[i]->sequenceID >= graph->nodes[j]->sequenceID) {
				graph->nodes[i]->adjacencyArray[graph->nodes[i]->adjacent] = graph->nodes[j];
				graph->nodes[i]->adjacent++;
			}
		}
	}
}