#ifndef _GBFS_H
#define _GBFS_H
#include "Includes.h"
#include "Heuristic.h"

/*
* Finds the shortest path to an enterence given the current player's starting point.
* Greedy Best First Search (GBFS) is essentially the same as A*, just without the g value.
*/
struct Stack* GBFS(struct Graph* graph);

#endif