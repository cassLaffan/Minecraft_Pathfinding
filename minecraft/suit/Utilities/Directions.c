#undef _WINSOCKAPI_
#define _WINSOCKAPI_
#include "Directions.h"
#include "MinecraftClient.h"
#define M_PI 3.14159265358979323846

float relativeToPlayer(struct Location* location, struct Node* node) {
	float gx = node->x - location->x;
	float gz = node->z - location->z;

	//flipping the Z axis so the atan2 function works
	float beta = atan2(-gz, gx) + M_PI;
	
	float delta = ((3 * M_PI)/2) - beta;
	float alpha = delta - location->yaw;

	if (alpha < 0) {
		alpha = alpha + 2*M_PI;
	}

	return alpha;
}

float getDistance(struct Location* a, struct Node* b) {
	return sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2) + pow(a->z - b->z, 2));
}


void getInstructions(float angle) {

	if (angle < (M_PI)+0.5 && angle >(M_PI) - 0.5) {
		printf("Forward!\n");
	}
	else if (angle > M_PI / 2 - 0.5 && angle < M_PI - 0.5) {
		printf("To your left\n");
	}
	else if (angle > M_PI + 0.5 && angle < 3 * M_PI / 2 + 0.5) {
		printf("To your right\n");
	}
	else {
		printf("Backwards or turn around. I'm not your dad.\n");
	}

}


void giveDirections(struct Stack* stack) {
	struct Location* loc = (struct Location*)malloc(sizeof(struct Location*));
	mcGetLocation(loc);
	//why do I need to pop this???? wtf
	pop(stack);
	struct Node* node = pop(stack);
	struct Node* lastNode = node;
	float angle = 0;

	while (!isStackEmpty(stack)) {
		mcStartUpsertGraph();
		mcUpsertNode(node->ID, 0, 0, 0, 0xFF'41'FF'FF, 0.025F);
		mcUpsertEdge(node->ID, lastNode->ID, 0xFF'41'FF'FF);
		mcStopUpsertGraph();

		while (getDistance(loc, node) > 2.0) {
			printf("Next node: %d\n", node->sequenceID);
			angle = relativeToPlayer(loc, node);

			getInstructions(angle);
			Sleep(1000);
			mcGetLocation(loc);
		}
		Sleep(1000);
		lastNode = node;
		node = pop(stack);
		mcGetLocation(loc);
	}
}

