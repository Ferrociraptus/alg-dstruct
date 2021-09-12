#ifndef SIMPLE_TESTING_OBJECTS_H
#define SIMPLE_TESTING_OBJECTS_H

extern "C"{
#include "Object.h"
}

#define INT_OBJECT(obj) ((IntObject*)(obj))

// Simple object for testing list
typedef struct{
	Object obj;
	int val;
} IntObject;

MEMORY_ALLOCATOR(IntObject);
// Simple IntObject constructor
CONSTRUCTOR(IntObject, int value);

#endif
