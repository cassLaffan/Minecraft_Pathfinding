#include "UserInstructions.h"

struct Stack* run(struct Graph* graph) {
	int choice = -1;
	int weight = 0;

	while (choice < 0 || choice > 5) {	
		printf("Please choose an algorithm to run:\n");
		printf("0 - Dijkstra\n");
		printf("1 - A*\n");
		printf("2 - Weighted A*\n");
		printf("3 - Greedy Best First Search\n");
		printf("4 - IDA* (only for datasets with one user)\n");
		printf("5 - Recursive Best First Search\n");

		scanf_s("%d", &choice);

		switch (choice) {
			case 0:
				// Weight = 0, making it dijkstra, gbfs = 1 making sure g is multiplied by 1
				return aStarGBFS(graph, 0, 1);
			case 1:
				// Weight = 1, making it A*, gbfs = 1 making sure g is multiplied by 1
				return aStarGBFS(graph, 1, 1);
			case 2:
				printf("Choose a positive weight. If you input 0, you'll just end up with Dijkstra.\n");
				scanf_s("%d", &weight);
				return aStarGBFS(graph, weight, 1);
			case 3:
				// Weight = 1, gbfs = 0 ensuring that g becomes 0
				return aStarGBFS(graph, 1, 1);
			case 4:
				printf("This is a you problem if this is a dataset with more than one user.\n");
				return reverseStack(IDAStar(graph));
			case 5: 
				return RBFS(graph);
			default:
				printf("Well screw you, none of those inputs were correct.\n");
		}
	}

}
