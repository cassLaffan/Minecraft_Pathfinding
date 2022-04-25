#ifndef _DIRECTIONS_H
#define _DIRECTIONS_H
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include "Includes.h"
#include "Graph.h"
#include "AStar.h"
#include "GBFS.h"
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