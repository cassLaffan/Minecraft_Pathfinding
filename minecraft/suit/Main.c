#include "MinecraftClient.h"

int main() {
	struct Graph* graph = createGraph();

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
	printf("At least we have connected.");

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

	printGraph(graph);

	struct Stack* pathBack = aStarRecreation(graph);
	struct Node* current;
	struct Node* next;

	printGraph(graph);
	mcStartUpsertGraph();

	for (int i = pathBack->top - 1; i > 0; i--) {
		current = pathBack->array[i];
		next = pathBack->array[i - 1];
		printf("%d->%d\n", current->sequenceID, next->sequenceID);
		mcUpsertNode(current->ID, 0, 0, 0, GREEN, 0.025);
		mcUpsertEdge(next->ID, current->ID, GREEN);
	}

	// Upsert last node (`next` in the last iteration is never drawn)
	// Breaks upon finding an empty stack. Maybe I should ass some error checking?
	mcUpsertNode(pathBack->array[0]->ID, 0, 0, 0, GREEN, 0.025);

	mcStopUpsertGraph();

	giveDirections(pathBack);

	printf("Number of unique expansions: %d\n", graph->expansions);
	printf("Number of unique expansions: %d\n", graph->reExpansions);

	freeStack(pathBack);
	free(graph);

	return 0;
}