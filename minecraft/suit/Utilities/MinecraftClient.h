#ifndef _MC_CLIENT_H
#define _MC_CLIENT_H

//#pragma once
//#include <winsock2.h>

#include "../Data Structures/Graph.h"
#include "Directions.h"
#include "Metrics.h"

int mcInit();
int mcConnect(char* ip, int port);
int mcClose();

void mcGetLocation(struct Location* location);
int mcGetPlayerID();
void mcGetNodes(struct Graph* graph);

void mcStartUpsertGraph();
void mcUpsertNode(int id, float x, float y, float z, int color, float size);
void mcUpsertEdge(int aId, int bId, int color);
void mcStopUpsertGraph();

void mcResetGraph();

#endif
