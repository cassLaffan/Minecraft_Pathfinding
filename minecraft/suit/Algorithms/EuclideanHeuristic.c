#include "EuclideanHeuristic.h"

void euclideanComputeH(struct Graph* graph) {
	for (int i = graph->used - 1; i >= 0; i--) {
		float sum = 0;

		for (int j = 0; j < graph->numUsers; j++) {
			sum += distance(graph->nodes[i], graph->starts[j]);
		}

		graph->nodes[i]->h = sum / graph->numUsers;
	}
}