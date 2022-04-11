#include "Directions.h"
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

	struct Stack* pathBack = simplePathRecreation(graph);
	struct Node* current;
	struct Node* next;


	mcStartUpsertGraph();

	for (int i = pathBack->top - 1; i > 0; i--) {
		current = pathBack->array[i];
		next = pathBack->array[i - 1];
		printf("%d->%d\n", current->ID, next->ID);
		mcUpsertNode(current->ID, 0, 0, 0, GREEN, 0.025);
		mcUpsertEdge(next->ID, current->ID, GREEN);
	}

	// Upsert last node (`next` in the last iteration is never drawn)
	current = pathBack->array[0];
	mcUpsertNode(current->ID, 0, 0, 0, GREEN, 0.025);

	mcStopUpsertGraph();

	giveDirections(pathBack);

	freeStack(pathBack);
	free(graph);
	remove("ARAIG_VisualizationInformation.txt");

	return 0;
}