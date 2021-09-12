#include <stdlib.h>
#include "Object.h"

void* __empty(void* self){
	return self;
}

Object* __del(Object* this) {
	if (this != NULL)
		return OBJECT((*this->destructor)(this));
	return this;
};

DEFAULT_MEMORY_ALLOCATOR(Object);

CONSTRUCTOR(Object){
	if (self == NULL)
		return NULL;
	REGISTER_DESTRUCTOR(self, __empty);
	return self;
};
