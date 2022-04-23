#ifndef _METRICS_H
#define _METRICS_H

#include "Includes.h"

/*
* This file is subject to change. Why? Becuase this functionality is for Windows.
* Happy 47th birthday, Microsoft. I hope it was worth it.
* All functionality inspired/guided by: https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
*/


/*
* Memory functions. The integer option asks if the value should
* represent the virutal or physical memory. 0 is for virtual, 1 is
* for physical.
*/
DWORDLONG getCurrent(int option);
DWORDLONG getTotal(int option);
size_t getCurrentProcess(int option);



/*
* CPU Usage
*/
double getCurrentCPU();


#endif