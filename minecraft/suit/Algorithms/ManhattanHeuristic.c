#include "ManhattanHeuristic.h"

void manComputeH(struct Graph* graph, struct Node* node) {
	float sum = 0;

	for (int j = 0; j < graph->numUsers; j++) {
		sum += abs((node->x + graph->starts[j]->x)) + abs((node->z + graph->starts[j]->z));
	}

	node->h = sum / graph->numUsers;
	
}
