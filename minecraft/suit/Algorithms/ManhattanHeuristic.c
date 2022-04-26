#include "ManhattanHeuristic.h"

void computeH(struct Graph* graph) {
	for (int i = graph->used - 1; i >= 0; i--) {
		float sum = 0;

		for (int j = 0; j < graph->numUsers; j++) {
			sum += abs((graph->nodes[i]->x + graph->starts[j]->x)) + abs((graph->nodes[i]->y + graph->starts[j]->y)) + abs((graph->nodes[i]->z + graph->starts[j]->z));
		}

		graph->nodes[i]->h = sum / graph->numUsers;
	}
}