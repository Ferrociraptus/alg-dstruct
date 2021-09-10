#include <stdio.h>
#include <inttypes.h>
#include "ObjectArrayStack.h"

struct __object_array_stack{
	Object obj;
	Object** arr;
	uint64_t size;
};

static DESTRUCTOR(ObjectArrayStack){
	free(self->arr);
	return self;
}

DEFAULT_MEMORY_ALLOCATOR(ObjectArrayStack);

CONSTRUCTOR(ObjectArrayStack){
	if (self == NULL)
		return NULL;
	
	REGISTER_DESTRUCTOR(self, DESTRUCTOR_FUN(ObjectArrayStack));
	self->size = 0;
	
	self->arr = malloc(sizeof(Object*));
	if (self->arr == NULL){
		free(self);
		return NULL;
	}
	return self;
}

void object_array_stack_add(ObjectArrayStack* stack, Object* obj){
	Object** resize_p = realloc(stack->arr, sizeof(Object*) * (stack->size + 1));
	if (resize_p == NULL)
		return;
	
	stack->arr = resize_p;
	stack->arr[stack->size] = obj;
	stack->size++;
}

Object* object_array_stack_pop(ObjectArrayStack* stack){
	if (stack->size <= 0)
		return NULL;
	
	Object* return_obj = stack->arr[stack->size - 1];
	
	if (stack->size > 1){
		Object** resize_p = realloc(stack->arr, sizeof(Object*) * (stack->size - 1));
		if (resize_p == NULL)
			return NULL;
		
		stack->arr = resize_p;
	}
	stack->size--;
	
	return return_obj;
}

unsigned long long object_array_stack_size(ObjectArrayStack* stack){
	return stack->size;
}
