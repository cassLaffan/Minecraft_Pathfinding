#include "AStar.h"

// Note: when wanting to deal with the number of nodes preceeding the current node
// having a weight of 0 < w < 1 will be the way to go
struct Stack* aStarRecreation(struct Graph* graph, int weight) {

	//Creates an empty queue for the A* algorithm
	struct Queue* priorityQueue = createQueue(graph->used * 8);

	//Add the first node to the queue with an h value of 0
	struct Node* u = graph->nodes[graph->used - 1];
	euclideanComputeH(graph, u);
	enqueue(priorityQueue, u, 0);

	while (!isEmpty(priorityQueue)) {
		//dequeues whatever node has top priority
		u = dequeue(priorityQueue);
		// means it has found one of the enterences/exits
		if (u->sequenceID == 0) {
			u->isFinish = 1;
			break;
		}

		for (int i = 0; i < u->adjacent; i++) {
			euclideanComputeH(graph, u->adjacencyArray[i]);
			if (!u->adjacencyArray[i]->visited || u->adjacencyArray[i]->g + distance(u->adjacencyArray[i], u) < u->adjacencyArray[i]->g) {
				if (u->adjacencyArray[i]->visited) {
					graph->reExpansions++;
				}
				else {
					graph->expansions++;
					u->adjacencyArray[i]->visited = 1;
				}
				u->adjacencyArray[i]->previous = u;
				//updating the cost to get to node u
				u->adjacencyArray[i]->g = u->adjacencyArray[i]->previous->g + distance(u->adjacencyArray[i], u);
				u->adjacencyArray[i]->f = u->adjacencyArray[i]->g + 10 * u->adjacencyArray[i]->h;
				enqueue(priorityQueue, u->adjacencyArray[i], u->adjacencyArray[i]->f);
			}
		}
	}

	// Creates the stack necessary to navigate back.
	struct Stack* path = createStack(graph->used);

	push(path, u);
	while (u != graph->nodes[graph->used - 1]){
		u = u->previous;
		push(path, u);
	}

	return path;
}