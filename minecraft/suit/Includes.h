#ifndef _INCLUDES_H
#define _INCLUDES_H

#undef _WINSOCKAPI_
#define _WINSOCKAPI_
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <psapi.h>
#include <time.h>
#include <float.h>
#include <limits.h> 
#include <TCHAR.h>
#include <pdh.h>
/*
* This file is here because it was part of the SDK and I'm afraid to take it out.
* Now it's more of a "this is where I put all the include calls because it means cleaner other files"
*/

typedef struct Vector3f{
	float x;
	float y;
	float z;
};

#endif
