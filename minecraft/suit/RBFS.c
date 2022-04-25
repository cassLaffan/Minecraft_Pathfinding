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

	int index = 0;
	struct Node* best = node->adjacencyArray[0];
	struct Node* result = node->adjacencyArray[0];
	struct Node* alternative = best;

	//ADD PREVIOUS MODIFICATION AND WAY TO WORK WAY BACK UP RECURSION
	while (result != NULL && !result->isFinish && index < node->adjacent) {

		if (node->adjacencyArray[node->adjacent - 1] != best) {
			alternative = node->adjacencyArray[index + 1];
		}

		if (node->adjacencyArray[0]->f > limit) {
			return NULL;
		}

		if (best->visited) {
			graph->reExpansions++;
		}
		else {
			graph->expansions++;
			best->visited = 1;
		}

		if (alternative->f < limit) {
			result = RBFSHelper(graph, best, alternative->f);
		}
		else {
			result = RBFSHelper(graph, best, limit);
		}

		if (!result) {
			return result;
		}

		else if (result->isFinish) {
			printf("Flag\n");
			node->isFinish = 1;
			best->previous = node;
			return node;
		}

		else {
			limit = result->f;
		}

		index++;
		best = node->adjacencyArray[index];
	}
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