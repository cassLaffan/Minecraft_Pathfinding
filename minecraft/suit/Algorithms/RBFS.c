#include "RBFS.h"

// This algorithm is simplified because I know that the only nodes without successors are the goal nodes
// And the function checks for that first
float RBFSHelper(struct Graph* graph, struct Node* node, float limit) {

	struct Queue* priorityQueue = createQueue(graph->used);

	for (int i = 0; i < node->adjacent; i++) {
		if(node->adjacencyArray[i]->sequenceID == 0){
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
		float f = node->adjacencyArray[i]->g + node->adjacencyArray[i]->h;

		//successor.f <- max(successor.g + successor.h, node.f))
		if (node->f > f) {
			node->adjacencyArray[i]->f = node->f;
		}
		else {
			node->adjacencyArray[i]->f = f;
		}
		enqueue(priorityQueue, node->adjacencyArray[i], node->adjacencyArray[i]->f);
	}


	struct Node* best = dequeue(priorityQueue);
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
		if (isEmpty(priorityQueue)) {
			best->f = RBFSHelper(graph, best, limit);
			break;
		}
		else {
			//messy work around for peaking
			temp = dequeue(priorityQueue);
			alternative = temp->f;
			enqueue(priorityQueue, temp, temp->f);
			best->f = RBFSHelper(graph, best, alternative);
		}

		if (best->isFinish) {
			printf("Finish");
			best->previous = node;
			node->isFinish = 1;
			return best->f;
		}
		//enqueue(priorityQueue, best, best->f);
		best = dequeue(priorityQueue);
	}
	freeQueue(priorityQueue);
	return best->f;
}

struct Stack* RBFS(struct Graph* graph) {
	//Creates the adjecencies by using the octree
	findAdjecencies(graph);

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