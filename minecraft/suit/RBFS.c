#include "RBFS.h"

void swapNodes(struct Node* first, struct Node* second) {
	struct Node* temp = first;
	first = second;
	second = temp;
}

void selectionSort(struct Node* node) {
	int min;
	for (int i = 0; i < node->adjacent - 1; i++) {
		min = i;

		for (int j = i + 1; j < node->adjacent; j++)
			if (node->adjacencyArray[j] < node->adjacencyArray[min])
				min = j;

		swapNodes(&node->adjacencyArray[min], &node->adjacencyArray[i]);
	}
}


// This algorithm is simplified because I know that the only nodes without successors are the goal nodes
// And the function checks for that first
struct Node* RBFSHelper(struct Graph* graph, struct Node* node, float limit) {
	if(node->sequenceID == 0){
		node->isFinish = 1;
		return node;
	}


	for (int i = 1; i < node->adjacent; i++) {

		// need a meaningful g value
		if (node->adjacencyArray[i]->g == FLT_MAX) {
			node->adjacencyArray[i]->g = distance(node, node->adjacencyArray[i]) + node->g;
		}

		//successor.f <- max(successor.g + successor.h, node.f))
		if (node->f > (node->adjacencyArray[i]->g + node->adjacencyArray[i]->h)) {
			node->adjacencyArray[i]->f = node->f;
		}
		else {
			node->adjacencyArray[i]->f = node->adjacencyArray[i]->g + node->adjacencyArray[i]->h;
		}
	}

	selectionSort(node);

	struct Node* best = node->adjacencyArray[0];
	struct Node* alternative = best;

	if (node->adjacent > 0){
		alternative = node->adjacencyArray[1];
	}

	while (!best->isFinish && best != NULL) {
		if (node->adjacencyArray[0]->f > limit) {
			return NULL;
		}

		if (alternative->f < limit) {
			if (alternative->visited) {
				graph->reExpansions++;
			}
			else {
				graph->expansions++;
				alternative->visited = 1;
			}
			best = RBFSHelper(graph, best, alternative->f);
		}
		else {
			if (alternative->visited) {
				graph->reExpansions++;
			}
			else {
				graph->expansions++;
				alternative->visited = 1;
			}
			best = RBFSHelper(graph, best, limit);
		}
	}
	return best;
}

struct Stack* RBFS(struct Graph* graph) {
	//Creates the adjecencies by using the octree
	findAdjecencies(graph);

	//Computes all the H values
	computeH(graph);

	struct Node* u = graph->nodes[graph->used - 1];
	u->f = u->h;

	float limit = FLT_MAX;

	RBFSHelper(graph, u, limit);
	

	// Creates the stack necessary to navigate back.
	struct Stack* path = createStack(graph->used);

	u = graph->nodes[0];

	for (int i = 0; i < graph->numUsers; i++) {
		u = graph->starts[i]->isFinish ? graph->starts[i] : u;
	}

	push(path, u);
	while (u != graph->nodes[graph->used - 1]) {
		u = u->previous;
		push(path, u);
	}

	return path;
}