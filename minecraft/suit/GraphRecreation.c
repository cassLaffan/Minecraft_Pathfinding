#include <stdio.h>
#include <stdlib.h>
#include "GraphRecreation.h"

void computeH(struct Graph* graph) {
	
}

// Default A* algorithm which uses the Euclidean distance heuristic
struct Stack* aStarRecreation(struct Graph* graph) {
	//Creates the adjecencies by using the octree
	findAdjecencies(graph);

	//Creates an empty queue for the A* algorithm
	struct Queue* priorityQueue = createQueue(graph->used * 8);
	struct Node* current;
	int place = 0;
	int length = 0;

	//Add the first node to the queue with an h value of 0
	struct Stack* path = createStack(graph->used);
	enqueue(priorityQueue, graph->nodes[graph->used-1], 0);

	while (!isEmpty(priorityQueue)) {
		//dequeues whatever node has top priority
		struct Node* u = dequeue(priorityQueue);
		printf("Sequence ID: %d\n", u->sequenceID);
		// means it has found one of the enterences/exits
		if (u->sequenceID == 0) {
			break;
		}


		for (int i = 0; i < u->adjacent; i++) {
			if (distance(u->adjacencyArray[i], u) < u->g) {
				u->adjacencyArray[i]->previous = u;
				//updating the cost to get to node u
				u->adjacencyArray[i]->g = (u->adjacencyArray[i]->g - distance(u->adjacencyArray[i], u->adjacencyArray[i]->previous)) + distance(u->adjacencyArray[i], u);
				u->adjacencyArray[i]->f = u->adjacencyArray[i]->g + u->adjacencyArray[i]->h;
				enqueue(priorityQueue, u->adjacencyArray[i], u->adjacencyArray[i]->f);
				if (u->visited) {
					u->adjacencyArray[i]->reExpansions++;
				}
			}
		}

	struct Stack* newPath = createStack(graph->used);


	return newPath;
}