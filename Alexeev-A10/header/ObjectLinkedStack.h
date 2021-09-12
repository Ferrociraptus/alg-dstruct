#ifndef OBJECT_LINKED_STACK_H
#define OBJECT_LINKED_STACK_H

#include "Object.h"
#define OBJECT_LINKED_STACK(obj) ((ObjectLinkedStack*)(obj))

typedef struct __object_linked_stack ObjectLinkedStack;

MEMORY_ALLOCATOR(ObjectLinkedStack);
CONSTRUCTOR(ObjectLinkedStack);
void object_linked_stack_add(ObjectLinkedStack* stack, Object* obj);
Object* object_linked_stack_pop(ObjectLinkedStack* stack);
unsigned long long object_linked_stack_size(ObjectLinkedStack* stack);

#endif
