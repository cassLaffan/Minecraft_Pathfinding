#include "Heuristic.h"

//current heuristic is the average of the distances a node has to each enterence/exit node.
void computeH(struct Graph* graph) {
	for (int i = graph->used - 1; i >= 0; i--) {
		float sum = 0;

		for (int j = 0; j < graph->numUsers; j++) {
			sum += distance(graph->nodes[i], graph->starts[j]);
		}

		graph->nodes[i]->h = sum / graph->numUsers;
	}
}