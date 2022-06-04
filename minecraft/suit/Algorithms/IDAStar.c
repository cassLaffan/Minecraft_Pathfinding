#include "IDAStar.h"

void searchForPlan(struct Graph* graph, struct Stack* path, struct Node* node) {
    push(path, node);//adding the first node

    double bound = node->h;
    double flag = 0;

    while (1) {
        flag = searchHelper(graph, path, 0, bound);
        if (flag == -1.0) {
            return 1;
        }
        else if (flag == INFINITY) {
            return -1;
        }
        bound = 10 * flag;
    }
}

float searchHelper(struct Graph* graph, struct Stack* path, float g, float bound){
    struct Node* current = peek(path);
    float min = INFINITY;
    float currH = current->h;

    float f = currH + g;
    float flag = 0;

    if (f > bound){
        return f;
    }

    if (current->sequenceID == 0){
        current->isFinish = 1;
        return -1.0;
    }

    for (int i = 0; i < current->adjacent; i++){
        euclideanComputeH(graph, current->adjacencyArray[i]);
        float cost = distance(current, current->adjacencyArray[i]);
        float nextG = g + cost;
        struct Node* next = current;

        if (!find(path, current->adjacencyArray[i])){
            if (current->adjacencyArray[i]->visited) {
                graph->reExpansions++;
            }
            else {
                graph->expansions++;
                current->adjacencyArray[i]->visited = 1;
            }

            push(path, current->adjacencyArray[i]);
            flag = searchHelper(graph, path, nextG, bound);
            if (flag == -1.0){
                current->adjacencyArray[i]->previous = current;
                return -1.0;
            }
            else if (flag < min){
                min = flag;
            }
            pop(path);
        }
    }
    return min;
}

struct Stack* IDAStar(struct Graph* graph) {

	//Add the first node to the queue with an h value of 0
	struct Node* u = graph->nodes[graph->used - 1];
    euclideanComputeH(graph, u);

	// Creates the stack necessary to navigate back.
	struct Stack* path = createStack(graph->used * 10);


    searchForPlan(graph, path, u);

    //reverseStack(path);

	return path;
}
