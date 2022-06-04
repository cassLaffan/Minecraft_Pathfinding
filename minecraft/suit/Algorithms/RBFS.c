#include "RBFS.h"

// This algorithm is simplified because I know that the only nodes without successors are the goal nodes
// And the function checks for that first
float RBFSHelper(struct Graph* graph, struct Node* node, float limit) {

	for (int i = 0; i < node->adjacent; i++) {
		if (node->adjacencyArray[i]->sequenceID == 0) {
			node->adjacencyArray[i]->isFinish = 0;
			node->adjacencyArray[i]->previous = node;
			node->isFinish = 1;
			return node->f;
		}
		euclideanComputeH(graph, node->adjacencyArray[i]);
		// need a meaningful g value
		float temp_g = distance(node, node->adjacencyArray[i]);
		if (temp_g + node->g < node->adjacencyArray[i]->g) {
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
		enqueue(node->priorityQueue, node->adjacencyArray[i], node->adjacencyArray[i]->f);
	}

	struct Node* best = dequeue(node->priorityQueue);
	struct Node* temp;
	float alternative;

	while (best->f < INFINITY && best->f <= limit && !best->isFinish) {
		if (best->f > limit) {
			return best->f;
		}
		//metrics
		if (best->visited) {
			graph->reExpansions++;
		}
		else {
			graph->expansions++;
			best->visited = 1;
		}

		//algorithm
		if (isEmpty(node->priorityQueue)) {
			best->f = RBFSHelper(graph, best, limit);
			break;
		}
		else {
			//messy work around for peaking
			temp = dequeue(node->priorityQueue);
			alternative = temp->f;
			enqueue(node->priorityQueue, temp, temp->f);
			best->f = RBFSHelper(graph, best, alternative);
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
	addQueues(graph);
	struct Node* u = graph->nodes[graph->used - 1];
	euclideanComputeH(graph, u);
	u->f = u->h;
	u->g = 0;

	float limit = INFINITY;
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
