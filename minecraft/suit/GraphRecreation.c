#include <stdio.h>
#include <stdlib.h>
#include "GraphRecreation.h"

struct Stack* aStarRecreation(struct Graph* graph) {
	//Creates the adjecencies by using the octree
	findAdjecencies(graph);

	//Creates an empty queue for the A* algorithm
	struct Queue* priorityQueue = createQueue(graph->used * 8);
	struct Node* current;
	int place = 0;
	int length = 0;

	//Add the first node to the queue with a distance to itself of 0
	struct Stack* path = createStack(graph->used);
	enqueue(priorityQueue, graph->nodes[0], 0);

	while (!isEmpty(priorityQueue)) {
		//dequeues whatever node has top priority
		struct Node* u = dequeue(priorityQueue);
		printf("Sequence ID: %d\n", u->sequenceID);
		if (u->sequenceID == 0) {
			break;
		}

		//for each next node connected to this node, we check its distance from the final node
		//The euclidean heuristic is what makes it A*
		for (int i = 0; i < u->adjacent; i++) {
			if (distance(u->adjacencyArray[i], u) < distance(u->adjacencyArray[i], u->adjacencyArray[i]->previous)) {
				u->adjacencyArray[i]->previous = u;
				u->adjacencyArray[i]->currentDistance = (u->adjacencyArray[i]->currentDistance - distance(u->adjacencyArray[i], u->adjacencyArray[i]->previous)) + distance(u->adjacencyArray[i], u);
			}
			if (!u->adjacencyArray[i]->visited) {
				u->adjacencyArray[i]->visited = 1;
				enqueue(priorityQueue, u->adjacencyArray[i], distance(u->adjacencyArray[i], graph->nodes[graph->used - 1]));
				}
			}

		}

	struct Stack* newPath = createStack(graph->used);

	return newPath;
}