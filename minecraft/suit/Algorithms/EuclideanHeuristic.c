#include "EuclideanHeuristic.h"

void euclideanComputeH(struct Graph* graph, struct Node* node) {
	float sum = 0;

	for (int j = 0; j < graph->numUsers; j++) {
		sum += distance(node, graph->starts[j]);
	}

	node->h = sum / graph->numUsers;
	
}