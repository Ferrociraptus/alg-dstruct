#ifndef OBJECT_ARRAY_STACK_H
#define OBJECT_ARRAY_STACK_H

#include "Object.h"

#define OBJECT_ARRAY_STACK(obj) ((ObjectArrayStack*)(obj))

typedef struct __object_array_stack ObjectArrayStack;

MEMORY_ALLOCATOR(ObjectArrayStack);
CONSTRUCTOR(ObjectArrayStack);
void object_array_stack_add(ObjectArrayStack* stack, Object* obj);
Object* object_array_stack_pop(ObjectArrayStack* stack);
unsigned long long object_array_stack_size(ObjectArrayStack* stack);

#endif
