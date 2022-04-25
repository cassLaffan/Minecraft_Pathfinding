#include "IDAStar.h"

void searchForPlan(struct Stack* path, struct Node* node) {
    push(path, node);//adding the first node

    double bound = node->h;
    double flag = 0;

    while (1) {
        flag = searchHelper(path, 0, bound);
        if (flag == -1.0) {
            return 1;
        }
        else if (flag == FLT_MAX) {
            return -1;
        }
        bound = flag;
    }
}

float searchHelper(struct Stack* path, float g, float bound){
    struct Node* current = peek(path);
    float min = FLT_MAX;
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
        float cost = distance(current, current->adjacencyArray[i]);
        float nextG = g + cost;
        struct Node* next = current;

        if (!find(path, current->adjacencyArray[i])){
            push(path, current->adjacencyArray[i]);
            flag = searchHelper(path, nextG, bound);
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
	//Creates the adjecencies by using the octree
	findAdjecencies(graph);
	//Computes all the H values
	computeH(graph);

	//Add the first node to the queue with an h value of 0
	struct Node* u = graph->nodes[graph->used - 1];

	// Creates the stack necessary to navigate back.
	struct Stack* path = createStack(graph->used * 10);


    searchForPlan(path, u);

    //reverseStack(path);

	return path;
}