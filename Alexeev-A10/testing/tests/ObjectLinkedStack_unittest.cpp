#include "gtest/gtest.h"

extern "C" {
#include <stdlib.h>
#include "ObjectLinkedStack.h"
}


#include "Simple_testing_objects.h"
/*
#define INT_OBJECT(obj) ((IntObject*)(obj))
// Simple object for testing list
typedef struct{
	Object obj;
	int val;
} IntObject;

// Simple IntObject constructor
IntObject* IntObject_new(int value);
IntObject* IntObject_new(int value){
	IntObject* obj = (IntObject*)malloc(sizeof(IntObject));
	obj->obj.destructor = DEFAULT_OBJ_DESTRUCTOR;
	obj->val = value;
	return obj;
}
*/



TEST(ObjectLinkedStack_functional_testing, stack_initialization_testing) {
	// initialize new stack with zero len
	ObjectLinkedStack* stack;
	ASSERT_NO_FATAL_FAILURE(stack = new(ObjectLinkedStack));
	
	ASSERT_EQ(object_linked_stack_size(stack), 0);
}


TEST(ObjectLinkedStack_functional_testing, empty_stack_destructor_testing) {
	// initialize new stack with zero len
	ObjectLinkedStack* stack = new(ObjectLinkedStack);
	
	ASSERT_NO_FATAL_FAILURE(del(stack));
}

TEST(ObjectLinkedStack_functional_testing, single_adding_item_to_stack_testing) {
	// initialize new stack with zero len
	ObjectLinkedStack* stack = new(ObjectLinkedStack);
	
	ASSERT_NO_FATAL_FAILURE(object_linked_stack_add(stack, OBJECT(new(IntObject, 1))));
	EXPECT_EQ(object_linked_stack_size(stack), 1);
}

TEST(ObjectLinkedStack_functional_testing, plural_adding_item_to_stack_testing) {
	// initialize new stack with zero len
	ObjectLinkedStack* stack = new(ObjectLinkedStack);
	
	ASSERT_NO_FATAL_FAILURE(object_linked_stack_add(stack, OBJECT(new(IntObject, 1))));
	ASSERT_NO_FATAL_FAILURE(object_linked_stack_add(stack, OBJECT(new(IntObject, 2))));
	ASSERT_NO_FATAL_FAILURE(object_linked_stack_add(stack, OBJECT(new(IntObject, 3))));
	
	EXPECT_EQ(object_linked_stack_size(stack), 3);
}

TEST(ObjectLinkedStack_functional_testing, not_empty_stack_destructor_testing) {
	// initialize new stack with zero len
	ObjectLinkedStack* stack = new(ObjectLinkedStack);
	
	object_linked_stack_add(stack, OBJECT(new(IntObject, 1)));
	object_linked_stack_add(stack, OBJECT(new(IntObject, 2)));
	object_linked_stack_add(stack, OBJECT(new(IntObject, 3)));
	
	ASSERT_NO_FATAL_FAILURE(del(stack));
}

TEST(ObjectLinkedStack_functional_testing, empty_stack_pop_testing) {
	// initialize new stack with zero len
	ObjectLinkedStack* stack = new(ObjectLinkedStack);
	
	EXPECT_EQ(object_linked_stack_pop(stack), nullptr);
	del(stack);
}

TEST(ObjectLinkedStack_functional_testing, not_empty_stack_pop_testing) {
	// initialize new stack with zero len
	ObjectLinkedStack* stack = new(ObjectLinkedStack);
	object_linked_stack_add(stack, OBJECT(new(IntObject, 1)));
	
	EXPECT_EQ(INT_OBJECT(object_linked_stack_pop(stack))->val, 1);
}
