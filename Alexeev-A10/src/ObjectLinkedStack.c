#include <stdlib.h>
#include "ObjectLinkedStack.h"
#include "ObjectLinkedList.h"

struct __object_linked_stack{
	Object obj;
	ObjectLinkedList* list;
};

static DESTRUCTOR(ObjectLinkedStack){
	if (self == NULL)
		return NULL;
	
	del(self->list);
	return self;
}

DEFAULT_MEMORY_ALLOCATOR(ObjectLinkedStack);

CONSTRUCTOR(ObjectLinkedStack){
	if (self == NULL)
		return NULL;
	
	REGISTER_DESTRUCTOR(self, DESTRUCTOR_FUN(ObjectLinkedStack));
	
	self->list = new(ObjectLinkedList, 0);
	if (self->list == NULL){
		del(self);
		return NULL;
	}
	return self;
}

void object_linked_stack_add(ObjectLinkedStack* stack, Object* obj){
	object_linked_list_append(stack->list, obj);
}

Object* object_linked_stack_pop(ObjectLinkedStack* stack){
	return object_linked_list_pop(stack->list, object_linked_list_len(stack->list)- 1);
}

unsigned long long object_linked_stack_size(ObjectLinkedStack* stack){
	return object_linked_list_len(stack->list);
}
