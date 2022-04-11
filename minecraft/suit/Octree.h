#ifndef _OCTREE_H
#define _OCTREE_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Includes.h"
#include "Graph.h"
#define M 4

struct Bounds {
	float xmin;
	float ymin;
	float zmin;
	float xmax;
	float ymax;
	float zmax;
};

struct Octree {
	struct Octree* children[8];
	//bounds for the box
	struct Bounds* bounds;
	int leaf;
	//subject to change
	int nodeNum;
	struct Node* nodes[M];
};

/*
* Creates a new bounds object based on a node's location and returns the bounds object.
*/
void makeBounds(struct Bounds* bound, struct Graph* graph);

/*
* Sets the bounds for a whole graph and ergo, its child node.
*/
void setBounds(struct Bounds* newBound, float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);

/*
* Splits octants into further octants. Returns a new bound object.
*/
void splitOctants(struct Bounds** newBounds, struct Bounds* bounds);

/*
* Checks if a node is within bounds. Returns 1 or 0.
*/
int isInBounds(struct Bounds* bounds, struct Node* node);

/*
* Initializes an empty octree and returns.
*/
struct Octree* initializeOctree(struct Bounds* bound);

/*
* Recursively creates and returns a new octree object.
*/
struct Octree* makeOctree(int num, struct Node** nodes, struct Bounds* bounds);

/*
* A helper function that checks neighbouring octants for potential adjacencies.
*/
int checkNearbyOctants(struct Bounds* bounds, struct Node* node);

/*
* The function that recursively searchest through an octree for neighbouring nodes.
*/
void recurseOctree(struct Octree* octree, struct Node* node);

/*
* Goes through the nodes of the graph to populate adjacency array
*/
void findAdjecencies(struct Graph* graph);

#endif