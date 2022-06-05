#include "UserInstructions.h"

struct Stack* runIndividual(struct Graph* graph) {

	int choice = -1;
	int weight = 0;

	while (choice < 0 || choice > 6) {	
		printf("Please choose an algorithm to run:\n");
		printf("0 - Dijkstra\n");
		printf("1 - A*\n");
		printf("2 - Weighted A*\n");
		printf("3 - Greedy Best First Search\n");
		printf("4 - IDA* (only for datasets with one user)\n");
		printf("5 - Recursive Best First Search\n");
		printf("6 - Run all (Except IDA*)\n");

		scanf_s("%d", &choice);
		clock_t begin, end;

		switch (choice) {
			case 0:
				// Weight = 0, making it dijkstra, gbfs = 1 making sure g is multiplied by 1
				begin = clock();
				findAdjecencies(graph);
				end = clock();
				printf("Time expended for creating the graph: %lf\n", (double)(end - begin) / CLOCKS_PER_SEC);

				return aStarGBFS(graph, 0, 1);

			case 1:
				// Weight = 1, making it A*, gbfs = 1 making sure g is multiplied by 1
				begin = clock();
				findAdjecencies(graph);
				end = clock();
				printf("Time expended for creating the graph: %lf\n", (double)(end - begin) / CLOCKS_PER_SEC);

				return aStarGBFS(graph, 1, 1);
			case 2:
				printf("Choose a positive weight. If you input 0, you'll just end up with Dijkstra.\n");
				scanf_s("%d", &weight);
				begin = clock();
				findAdjecencies(graph);
				end = clock();
				printf("Time expended for creating the graph: %lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
				return aStarGBFS(graph, weight, 1);
			case 3:
				// Weight = 1, gbfs = 0 ensuring that g becomes 0
				begin = clock();
				findAdjecencies(graph);
				end = clock();
				printf("Time expended for creating the graph: %lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
				return aStarGBFS(graph, 1, 1);
			case 4:
				printf("This is a you problem if this is a dataset with more than one user.\n");
				begin = clock();
				findAdjecencies(graph);
				end = clock();
				printf("Time expended for creating the graph: %lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
				return reverseStack(IDAStar(graph));
			case 5: 
				begin = clock();
				findAdjecencies(graph);
				end = clock();
				printf("Time expended for creating the graph: %lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
				return RBFS(graph);
			default:
				printf("Well screw you, none of those inputs were correct.\n");
		}
	}

}

void runAll(struct Graph* graph) {
	clock_t begin, end;

		// Weight = 0, making it dijkstra, gbfs = 1 making sure g is multiplied by 1
	printf("Dijkstra\n");
	begin = clock();
	findAdjecencies(graph);
	end = clock();
	printf("Time expended for creating the graph: %lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
	aStarGBFS(graph, 0, 1);
	printf("Number of unique expansions: %d\n", graph->expansions);
	printf("Number of re-expansions: %d\n", graph->reExpansions);
	resetGraph(graph);

	// Weight = 1, making it A*, gbfs = 1 making sure g is multiplied by 1
	printf("A*\n");
	begin = clock();
	findAdjecencies(graph);
	end = clock();
	printf("Time expended for creating the graph: %lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
	aStarGBFS(graph, 1, 1);
	printf("Number of unique expansions: %d\n", graph->expansions);
	printf("Number of re-expansions: %d\n", graph->reExpansions);
	resetGraph(graph);

	// Weight = 5
	printf("WA*, W = 5\n");
	begin = clock();
	findAdjecencies(graph);
	end = clock();
	printf("Time expended for creating the graph: %lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
	aStarGBFS(graph, 5, 1);
	printf("Number of unique expansions: %d\n", graph->expansions);
	printf("Number of re-expansions: %d\n", graph->reExpansions);
	resetGraph(graph);

	// Weight = 10
	printf("WA*, W = 10\n");
	begin = clock();
	findAdjecencies(graph);
	end = clock();
	printf("Time expended for creating the graph: %lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
	aStarGBFS(graph, 10, 1);
	printf("Number of unique expansions: %d\n", graph->expansions);
	printf("Number of re-expansions: %d\n", graph->reExpansions);
	resetGraph(graph);

	// Weight = 0.1
	printf("WA*, W = 0.1\n");
	begin = clock();
	findAdjecencies(graph);
	end = clock();
	printf("Time expended for creating the graph: %lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
	aStarGBFS(graph, 5, 1);
	printf("Number of unique expansions: %d\n", graph->expansions);
	printf("Number of re-expansions: %d\n", graph->reExpansions);
	resetGraph(graph);

	// Weight = 1, gbfs = 0 ensuring that g becomes 0
	printf("GBFS\n");
	begin = clock();
	findAdjecencies(graph);
	end = clock();
	printf("Time expended for creating the graph: %lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
	aStarGBFS(graph, 1, 1);
	printf("Number of unique expansions: %d\n", graph->expansions);
	printf("Number of re-expansions: %d\n", graph->reExpansions);
	resetGraph(graph);

	//RBFS
	printf("RBFS\n");
	begin = clock();
	findAdjecencies(graph);
	end = clock();
	printf("Time expended for creating the graph: %lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
	RBFS(graph);
	printf("Number of unique expansions: %d\n", graph->expansions);
	printf("Number of re-expansions: %d\n", graph->reExpansions);
	resetGraph(graph);
}
