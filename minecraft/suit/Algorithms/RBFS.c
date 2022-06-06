#include "RBFS.h"

// This algorithm is simplified because I know that the only nodes without successors are the goal nodes
// And the function checks for that first
float RBFSHelper(struct Graph* graph, struct Node* node, float limit) {

	for (int i = 0; i < node->adjacent; i++) {
		if (node->adjacencyArray[i]->sequenceID == 0) {
			node->adjacencyArray[i]->isFinish = 1;
			node->adjacencyArray[i]->previous = node;
			node->isFinish = 1;
			return node->f;
		}
		// need a meaningful g value
		float temp_g = distance(node, node->adjacencyArray[i]);
		if (temp_g + node->g <= node->adjacencyArray[i]->g) {
			node->adjacencyArray[i]->g = temp_g + node->g;
			node->adjacencyArray[i]->previous = node;
		}
		float f = node->adjacencyArray[i]->g + node->adjacencyArray[i]->h;

		//successor.f <- max(successor.g + successor.h, node.f))
		if (node->f > f) {
			node->adjacencyArray[i]->f = node->f;
		}
		else {
			node->adjacencyArray[i]->f = f;
		}
		//metrics
		if (node->adjacencyArray[i]->visited) {
			graph->reExpansions++;
		}
		else {
			graph->expansions++;
			node->adjacencyArray[i]->visited = 1;
		}

		enqueue(node->priorityQueue, node->adjacencyArray[i], node->adjacencyArray[i]->f);
	}

	struct Node* best = dequeue(node->priorityQueue);
	struct Node* temp;
	float alternative;



	while (best->f < INFINITY && !best->isFinish) {
		if (best->f > limit) {
			return best->f;
		}

		//algorithm
		if (isEmpty(node->priorityQueue)) {
			best->f = RBFSHelper(graph, best, limit);
		}
		//messy work around for peaking
		temp = dequeue(node->priorityQueue);
		alternative = temp->f;
		enqueue(node->priorityQueue, temp, temp->f);

		//need an educated guess here to limit searching down un promising branches
		if (alternative <= limit || (graph->reExpansions) > graph->used) {
			best->f = RBFSHelper(graph, best, alternative);
		}
		else {
			best->f = RBFSHelper(graph, best, limit);
		}
		

		if (best->isFinish) {
			best->previous = node;
			node->isFinish = 1;
			return best->f;
		}

		enqueue(node->priorityQueue, best, best->f);
		best = dequeue(node->priorityQueue);
	}
	return best->f;
}

struct Stack* RBFS(struct Graph* graph) {
	clock_t begin = clock();

	addQueues(graph);
	struct Node* u = graph->nodes[graph->used - 1];
	u->visited = 1;
	u->f = u->h;
	u->g = 0;

	float limit = INFINITY;
	RBFSHelper(graph, u, limit);


	// Creates the stack necessary to navigate back.
	struct Stack* path = createStack(graph->used);

	u = graph->nodes[0];

	clock_t end = clock();

	printf("Time expended for finding the egress path is: %lf\n", (double)(end - begin) / CLOCKS_PER_SEC);

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
