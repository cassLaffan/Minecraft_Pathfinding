#include "Octree.h"

/*
 * This returns a six element array with each of the bounds for the box
 * The format is: [max x, min x, max y, min y, max z, min z]
 */
void makeBounds(struct Bounds* bound, struct Graph* graph) {
    bound->xmax = graph->nodes[0]->x;
    bound->xmin = graph->nodes[0]->x;
    bound->ymax = graph->nodes[0]->y;
    bound->ymin = graph->nodes[0]->y;
    bound->zmax = graph->nodes[0]->z;
    bound->zmin = graph->nodes[0]->z;

    for (int i = 1; i < graph->used; i++) {
        if (graph->nodes[i]->x >= bound->xmax) {
            bound->xmax = graph->nodes[i]->x;
        }
        else if (graph->nodes[i]->x < bound->xmin) {
            bound->xmin = graph->nodes[i]->x;
        }
        if (graph->nodes[i]->y >= bound->ymax) {
            bound->ymax = graph->nodes[i]->y;
        }
        else if (graph->nodes[i]->y < bound->ymin) {
            bound->ymin = graph->nodes[i]->y;
        }
        if (graph->nodes[i]->z >= bound->zmax) {
            bound->zmax = graph->nodes[i]->z;
        }
        else if (graph->nodes[i]->z < bound->zmin) {
            bound->zmin = graph->nodes[i]->z;
        }
    }
}

void setBounds(struct Bounds* newBound, float xmin, float xmax, float ymin, float ymax, float zmin, float zmax) {
    newBound->xmax = xmax;
    newBound->xmin = xmin;
    newBound->ymax = ymax;
    newBound->ymin = ymin;
    newBound->zmax = zmax;
    newBound->zmin = zmin;
}

void splitOctants(struct Bounds** newBounds, struct Bounds* bounds) {

    float xHalf;
    float yHalf;
    float zHalf;

    if (bounds->xmin < 0) {
        xHalf = bounds->xmin + (fabs(bounds->xmax) + fabs(bounds->xmin)) / 2;
    }
    else {
        xHalf = (bounds->xmax + bounds->xmin) / 2.0;
    }
    if (bounds->ymin < 0) {
        yHalf = bounds->ymin + (fabs(bounds->ymax) + fabs(bounds->ymin)) / 2;
    }
    else {
        yHalf = (bounds->ymax + bounds->ymin) / 2.0;
    }
    if (bounds->zmin < 0) {
        zHalf = bounds->zmin + (fabs(bounds->zmax) + fabs(bounds->zmin)) / 2;
    }
    else {
        zHalf = (bounds->zmax + bounds->zmin) / 2.0;
    }


    //front half of the cube
    setBounds(newBounds[0], xHalf, bounds->xmax, bounds->ymin, yHalf, zHalf, bounds->zmax);
    setBounds(newBounds[1], bounds->xmin, xHalf, bounds->ymin, yHalf, zHalf, bounds->zmax);
    setBounds(newBounds[2], bounds->xmin, xHalf, yHalf, bounds->ymax, zHalf, bounds->zmax);
    setBounds(newBounds[3], xHalf, bounds->xmax, yHalf, bounds->ymax, zHalf, bounds->zmax);

    //back half of the cube
    setBounds(newBounds[4], xHalf, bounds->xmax, bounds->ymin, yHalf, bounds->zmin, zHalf);
    setBounds(newBounds[5], bounds->xmin, xHalf, bounds->ymin, yHalf, bounds->zmin, zHalf);
    setBounds(newBounds[6], bounds->xmin, xHalf, yHalf, bounds->ymax, bounds->zmin, zHalf);
    setBounds(newBounds[7], xHalf, bounds->xmax, yHalf, bounds->ymax, bounds->zmin, zHalf);

}

int isInBounds(struct Bounds* bounds, struct Node* node) {
    return (node->x >= bounds->xmin) && (node->x <= bounds->xmax) && (node->z >= bounds->zmin) && (node->z <= bounds->zmax) && (node->y >= bounds->ymin) && (node->y <= bounds->ymax);
}

struct Octree* initializeOctree(struct Bounds* bound) {
    struct Octree* octree = malloc(sizeof(struct Octree));

    for (int i = 0; i < 8; i++) {
        octree->children[i] = malloc(sizeof(struct Octree));
    }

    for (int i = 0; i < M; i++) {
        octree->nodes[i] = malloc(sizeof(struct Node));
    }
    octree->bounds = malloc(sizeof(struct Bounds*));
    octree->bounds = bound;
    octree->leaf = 0;
    octree->nodeNum = 0;

    return octree;
}

void freeOctree(struct Octree* octree) {

    for (int i = 0; i < 8; i++) {
        if (!octree->leaf && octree->children[0]) {
            freeOctree(octree->children[i]);
        }
    }

    free(octree->bounds);

    free(octree);
}

struct Octree* makeOctree(int num, struct Node** nodes, struct Bounds* bounds) {
    struct Octree* octree = initializeOctree(bounds);
    struct Bounds** newBoundaries = malloc(sizeof(struct Bounds*) * 8);

    if (!num) {
        octree->children[0] = NULL;
    }
    else if (num <= M) {
        octree->leaf = 1;
        for (int i = 0; i < num; i++) {
            octree->nodes[i] = nodes[i];
            octree->nodeNum++;
        }
    }
    else {
        //Split box into octants
        //return array of 8 new octants
        for (int i = 0; i < 8; i++) {
            newBoundaries[i] = malloc(sizeof(struct Bounds));
        }

        splitOctants(newBoundaries, bounds);
        //recurse into each octant
        for (int i = 0; i < 8; i++) {
            struct Node** newNodes = malloc(sizeof(struct Node*) * num);

            int place = 0;
            for (int j = 0; j < num; j++) {
                if (isInBounds(newBoundaries[i], nodes[j])) {
                    newNodes[place] = nodes[j];
                    place++;
                }
            }
            octree->children[i] = makeOctree(place, newNodes, newBoundaries[i]);

            free(newNodes);
        }
    }
    free(newBoundaries);
    return octree;
}

//Futile attempt to avoid code spaghetti
int checkNearbyOctants(struct Bounds* bounds, struct Node* node) {

    //Do not want to check adjacencies right above or right below. Don't want the firefighter hovering
    //That's witchcraft I'd like to avoid
    struct Node* newNodeOne = createNode(-1, -1, -1, node->x + 2, node->y, node->z);
    struct Node* newNodeTwo = createNode(-1, -1, -1, node->x - 2, node->y, node->z);
    struct Node* newNodeThree = createNode(-1, -1, -1, node->x, node->y, node->z + 2);
    struct Node* newNodeFour = createNode(-1, -1, -1, node->x, node->y, node->z - 2);

    struct Node* newNodeFive = createNode(-1, -1, -1, node->x + 2, node->y, node->z + 2);
    struct Node* newNodeSix = createNode(-1, -1, -1, node->x - 2, node->y, node->z + 2);
    struct Node* newNodeSeven = createNode(-1, -1, -1, node->x + 2, node->y, node->z - 2);
    struct Node* newNodeEight = createNode(-1, -1, -1, node->x - 2, node->y, node->z - 2);

    int isNear = (isInBounds(bounds, newNodeOne) || isInBounds(bounds, newNodeTwo) || isInBounds(bounds, newNodeThree) || isInBounds(bounds, newNodeFour)
        || isInBounds(bounds, newNodeFive) || isInBounds(bounds, newNodeSix) || isInBounds(bounds, newNodeSeven) || isInBounds(bounds, newNodeEight));

    free(newNodeOne);
    free(newNodeTwo);
    free(newNodeThree);
    free(newNodeFour);

    free(newNodeFive);
    free(newNodeSix);
    free(newNodeSeven);
    free(newNodeEight);

    return isNear;
}

void recurseOctree(struct Octree* octree, struct Node* node) {
    //base case: has reached a leaf with nodes
    if (octree->leaf) {
        for (int i = 0; i < octree->nodeNum; i++) {
            if (distance(node, octree->nodes[i]) < 4 && !inAdjacencies(node, octree->nodes[i]) && node->ID >= octree->nodes[i]->ID) {
                node->adjacencyArray[node->adjacent] = octree->nodes[i];
                node->adjacent++;
            }
        }
    }
    //recursive case: must recurse through the children
    else if (octree->children[0]) {
        for (int i = 0; i < 8; i++) {
            //check bounds, eliminate octants that aren't needed
            //need to add checking nearby octants
            if (isInBounds(octree->children[i]->bounds, node) || checkNearbyOctants(octree->children[i]->bounds, node)) {
                recurseOctree(octree->children[i], node);
            }
        }
    }
}

void findAdjecencies(struct Graph* graph) {
    struct Bounds* furthestBounds = malloc(sizeof(struct Bounds));


    makeBounds(furthestBounds, graph);
    struct Octree* space = makeOctree(graph->used, graph->nodes, furthestBounds);

    for (int i = 0; i < graph->used; i++) {
        recurseOctree(space, graph->nodes[i]);
    }

    //Frees furthestBounds by virtue of that pointer being part of the octree struct
    freeOctree(space);
}


