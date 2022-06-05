#ifndef _USER_INSTRUCTIONS_H
#define _USER_INSTRUCTIONS_H

#include "Directions.h"
#include "Metrics.h"
#include "../Includes.h"

/*
* Prompts user for which algorithm they would like to run.
* Then, runs it lol.
*/
struct Stack* runIndividual(struct Graph* graph);

/*
* Runs all of the algorithms because automation is the key to the future.
*/
void runAll(struct Graph* graph);

#endif
