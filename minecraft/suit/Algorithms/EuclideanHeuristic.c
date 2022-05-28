#include "EuclideanHeuristic.h"

void calculateWeights(struct Graph* graph) {
	int j = 0;
	for (int i = 0; i < graph->used; i++) {
		for (int start = 0; start < graph->numUsers; start++) {
			graph->nodes[j]->weightedAverage += distance(graph->nodes[i], graph->starts[j]);
		}
	}
}

// For classic minimum Euclidean distance from a set of goal nodes
// Admissible
void euclideanComputeH(struct Graph* graph, struct Node* node) {
	
	node->h = distance(node, graph->starts[0]);

	for (int j = 1; j < graph->numUsers; j++) {
		float h = distance(node, graph->starts[j]);
		
		if (h < node->h) {
			node->h = distance(node, graph->starts[j]);
		}
	}

	
	
}
