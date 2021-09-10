#include <stdlib.h>
#include "Simple_testing_objects.h"

DEFAULT_MEMORY_ALLOCATOR(IntObject);
CONSTRUCTOR(IntObject, int value){
// 	SUPER(Object)(OBJECT(self));
	Object_init(OBJECT(self));
	self->val = value;
	return self;
}
