#include "ZeroHeuristic.h"

void computeH(struct Graph* graph) {
	for (int i = graph->used - 1; i >= 0; i--) {
		graph->nodes[i]->h = (float)0;
	}
}