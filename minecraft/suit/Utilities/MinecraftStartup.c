#include "MinecraftStartup.h"

void startup(struct Graph* graph) {
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
}