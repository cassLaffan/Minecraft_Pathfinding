#ifndef _INCLUDES_H
#define _INCLUDES_H

#undef _WINSOCKAPI_
#define _WINSOCKAPI_
#include <stdio.h>
#include <windows.h>

/*
* This file is here because it's part of the SDK and I'm afraid to take it out.
*/

typedef struct Vector3f{
	float x;
	float y;
	float z;
};

#endif
