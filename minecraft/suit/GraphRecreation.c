#include <stdio.h>
#include <stdlib.h>
#include "GraphRecreation.h"

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

// Default A* algorithm which uses the Euclidean distance heuristic
struct Stack* aStarRecreation(struct Graph* graph) {
	//Creates the adjecencies by using the octree
	findAdjecencies(graph);

	//Computes all the H values
	computeH(graph);

	//Creates an empty queue for the A* algorithm
	struct Queue* priorityQueue = createQueue(graph->used * 8);

	//Add the first node to the queue with an h value of 0
	struct Node* u = graph->nodes[graph->used - 1];
	enqueue(priorityQueue, u, 0);

	while (!isEmpty(priorityQueue)) {
		//dequeues whatever node has top priority
		u = dequeue(priorityQueue);
		printf("Sequence ID: %d\n", u->sequenceID);
		// means it has found one of the enterences/exits
		if (u->sequenceID == 0) {
			u->isFinish = 1;
			break;
		}


		for (int i = 0; i < u->adjacent; i++) {
			if (!u->adjacencyArray[i]->visited || u->adjacencyArray[i]->g + distance(u->adjacencyArray[i], u) < u->adjacencyArray[i]->g) {
				u->adjacencyArray[i]->previous = u;
				//updating the cost to get to node u
				u->adjacencyArray[i]->g = u->adjacencyArray[i]->previous->g + distance(u->adjacencyArray[i], u);
				u->adjacencyArray[i]->f = u->adjacencyArray[i]->g + u->adjacencyArray[i]->h;
				enqueue(priorityQueue, u->adjacencyArray[i], u->adjacencyArray[i]->f);
				if (u->visited) {
					u->adjacencyArray[i]->reExpansions++;
				}
				else {
					u->visited = 1;
				}
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