#ifndef _ASTAR_H
#define _ASTAR_H
#include "../Includes.h"
#include "../Data Structures/Graph.h"
#include "../Data Structures/Stack.h"
#include "../Data Structures/AdjacencySearch.h"
#include "../Data Structures/Queue.h"
#include "EuclideanHeuristic.h"
#include "ZeroHeuristic.h"
#include "ManhattanHeuristic.h"

/*
* Finds the shortest path to an enterence given the current player's starting point.
* This is a combination of both A*, WA* and GBFS.
* If weight = 0, the algorithm becomes Dijkstra.
* gbfs is a flag that checks whether or not the function is GBFS. 
* For ease of use, 0 means it's GBFS and 1 means it's an A* algorithm.
*/

struct Stack* aStarGBFS(struct Graph* graph, int weight, int gbfs);

#endif
