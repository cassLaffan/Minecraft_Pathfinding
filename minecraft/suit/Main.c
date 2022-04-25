#include "Utilities/MinecraftClient.h"

int main() {
	struct Graph* graph = createGraph();

	//taking out minecraft connectivity for now

	/*
	if (mcInit() != 0) {
		printf("mcInit error\n");
		return 1;
	}

	if (mcConnect(25566) != 0) {
		printf("mcConnect  error\n");
		return 1;
	}

	mcResetGraph();
	mcGetNodes(graph);

	int WHITE = 0xFF'FF'FF'FF; // RGBA
	int GREEN = 0x00'FF'00'FF;

	mcStartUpsertGraph();


	for (int i = 0; i < graph->used; i++) {
		struct Node* node = graph->nodes[i];
		for (int j = 0; j < node->adjacent; j++) {
			mcUpsertEdge(node->ID, node->adjacencyArray[j]->ID, WHITE);
		}
	}
	mcStopUpsertGraph();
	*/

	FILE* file;
	fopen_s(&file, "Data Sets/test_points.txt", "r");
	char line[256];

	int nodeID, sequenceID;
	float x, y, z;

	do {
		fscanf_s(file, "%d %d %f %f %f", &nodeID, &sequenceID, &x, &y, &z);
		addNode(graph, createNode(nodeID, 0, sequenceID, x, y, z));
	} while (fgets(line, sizeof(line), file));

	fclose(file);

	struct Stack* pathBack = aStarRecreation(graph, 1);
	struct Node* current;
	struct Node* next;

	//cStartUpsertGraph();

	printGraph(graph);

	// Only for IDA*
	//struct Stack* pathBack = reverseStack(path);

	for (int i = pathBack->top; i > 0; i--) {
		current = pathBack->array[i];
		next = pathBack->array[i - 1];
		printf("%d->%d\n", current->sequenceID, next->sequenceID);
		//mcUpsertNode(current->ID, 0, 0, 0, GREEN, 0.025);
		//mcUpsertEdge(next->ID, current->ID, GREEN);
	}

	// Upsert last node (`next` in the last iteration is never drawn)
	// Breaks upon finding an empty stack. Maybe I should ass some error checking?
	// mcUpsertNode(pathBack->array[0]->ID, 0, 0, 0, GREEN, 0.025);

	mcStopUpsertGraph();

	//giveDirections(pathBack);

	//printGraph(graph);

	printf("Total physical memory used: %llu MB\n", getTotal(1) / 1024 / 1024);
	printf("Total virtual memory used: %llu MB\n", getTotal(0) / 1024 / 1024);
	//printf("Total CPU used: %lf", getCurrentCPU());

	printf("Number of unique expansions: %d\n", graph->expansions);
	printf("Number of re-expansions: %d\n", graph->reExpansions);

	freeStack(pathBack);
	free(graph);

	return 0;
}