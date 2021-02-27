#ifndef _SUIT_H
#define _SUIT_H
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "Includes.h"
#include <windows.h>
#include <math.h>
#include "Graph.h"
#include "GraphRecreation.h"
#include "Stack.h"

struct Location {
	float x;
	float y;
	float z;
	float yaw; //radians, as per math.h
};

/*
* Returns the angle of the next node relative to the player's yaw
*/
float relativeToPlayer(struct Location* location, struct Node* node);

/*
* Returns the distance of a player from a node using the power of trig
*/
float getDistance(struct Location* a, struct Node* b);

/*
* Gives a readout of directions to the screen.
*/
void getInstructions(float angle);

/*
* Gives live directions to the user.
*/
void giveDirections(struct Stack* stack);


#endif