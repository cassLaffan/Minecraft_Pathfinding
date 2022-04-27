#include "EuclideanHeuristic.h"

void calculateWeights(struct Graph* graph) {
	int j = 0;
	for (int i = 0; i < graph->used; i++) {
		for (int start = 0; start < graph->numUsers; start++) {
			graph->nodes[j]->weightedAverage += distance(graph->nodes[i], graph->starts[j]);
		}
	}
}

// For classic average Euclidean, weight is 1
void euclideanComputeH(struct Graph* graph, struct Node* node) {
	float sum = 0;

	for (int j = 0; j < graph->numUsers; j++) {
		sum += node->weightedAverage * distance(node, graph->starts[j]);
	}

	node->h = sum / graph->numUsers;
	
}