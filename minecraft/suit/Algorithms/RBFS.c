#include "RBFS.h"

float RBFSHelper(struct Queue* priorityQueue, struct Stack* stack, struct Graph* graph, struct Node* node, float limit) {
	float f = 0;

	for (int i = 0; i < node->adjacent; i++) {
		euclideanComputeH(graph, node->adjacencyArray[i]);
		if (node->adjacencyArray[i]->sequenceID == 0) {
			node->adjacencyArray[i]->isFinish = 0;
			node->adjacencyArray[i]->previous = node;
			node->isFinish = 1;
			return node->f;
		}
		// need a meaningful g value
		if (distance(node, node->adjacencyArray[i]) + node->g < node->adjacencyArray[i]->g) {
			node->adjacencyArray[i]->g = distance(node, node->adjacencyArray[i]) + node->g;
			node->adjacencyArray[i]->previous = node;
		}
		f = node->adjacencyArray[i]->g + node->adjacencyArray[i]->h;

		//successor.f <- max(successor.g + successor.h, node.f))
		if (node->f > f) {
			node->adjacencyArray[i]->f = node->f;
		}
		else {
			node->adjacencyArray[i]->f = f;
		}
		enqueue(priorityQueue, node->adjacencyArray[i], node->adjacencyArray[i]->f);
	}

	for (int i = 0; i < node->adjacent; i++) {
		push(stack, dequeue(priorityQueue));
	}


	struct Node* best = pop(stack);
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
		if (isStackEmpty(stack) || stack->top == 1) {
			best->f = RBFSHelper(priorityQueue, stack, graph, best, limit);
			break;
		}
		else {
			//messy work around for peaking
			alternative = stack->array[stack->top - 1]->f;
;			best->f = RBFSHelper(priorityQueue, stack, graph, best, alternative);
		}

		if (best->isFinish) {
			printf("GOAL %d\n", best->sequenceID);
			best->previous = node;
			node->isFinish = 1;
			return best->f;
		}
		//enqueue(priorityQueue, best, best->f);
		best = pop(stack);
	}
	return best->f;
}

struct Stack* RBFS(struct Graph* graph) {
	struct Node* u = graph->nodes[graph->used - 1];
	euclideanComputeH(graph, u);
	u->f = u->h;
	u->g = 0;
	struct Stack* stack = createStack(graph->used);
	struct Queue* queue = createQueue(graph->used);
	push(stack, u);

	float limit = INFINITY;
	RBFSHelper(queue, stack, graph, u, limit);


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