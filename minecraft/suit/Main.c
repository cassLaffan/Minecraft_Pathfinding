#include "Utilities/MinecraftClient.h"
#include "Utilities/OpenFile.h"
#include "Utilities/MinecraftStartup.h"
#include "Utilities/Directions.h"
#include "Utilities/UserInstructions.h"

int main() {
	struct Graph* graph = createGraph();

	//taking out minecraft connectivity for now
	//startup(graph);

	openAndUseFile(graph);

	clock_t begin = clock();

	// Use at own risk for gigantic datasets. This call can and will make your computer
	// eat dirt.
	// findOctreeAdjecencies(graph);

	findAdjecencies(graph);

	calculateWeights(graph);


	struct Stack* pathBack = run(graph);

	clock_t end = clock();
	double timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;

	struct Node* current;
	struct Node* next;

	//cStartUpsertGraph();

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

	printf("Total physical memory used: %llu MB\n", getTotal(1) / 1024 / 1024);
	printf("Total virtual memory used: %llu MB\n", getTotal(0) / 1024 / 1024);
	//printf("Total CPU used: %lf", getCurrentCPU());

	printf("Number of unique expansions: %d\n", graph->expansions);
	printf("Number of re-expansions: %d\n", graph->reExpansions);

	printf("Time expended: %lf\n", timeSpent);

	freeStack(pathBack);
	free(graph);

	return 0;
}
