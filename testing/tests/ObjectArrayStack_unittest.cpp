#include "gtest/gtest.h"

extern "C" {
#include <stdlib.h>
#include "ObjectArrayStack.h"
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



TEST(ObjectArrayStack_functional_testing, stack_initialization_testing) {
	// initialize new stack with zero len
	ObjectArrayStack* stack;
	ASSERT_NO_FATAL_FAILURE(stack = new(ObjectArrayStack));
	
	ASSERT_EQ(object_array_stack_size(stack), 0);
}


TEST(ObjectArrayStack_functional_testing, empty_stack_destructor_testing) {
	// initialize new stack with zero len
	ObjectArrayStack* stack = new(ObjectArrayStack);
	
	ASSERT_NO_FATAL_FAILURE(del(stack));
}

TEST(ObjectArrayStack_functional_testing, single_adding_item_to_stack_testing) {
	// initialize new stack with zero len
	ObjectArrayStack* stack = new(ObjectArrayStack);
	
	ASSERT_NO_FATAL_FAILURE(object_array_stack_add(stack, OBJECT(new(IntObject, 23))));
	EXPECT_EQ(object_array_stack_size(stack), 1);
}

TEST(ObjectArrayStack_functional_testing, plural_adding_item_to_stack_testing) {
	// initialize new stack with zero len
	ObjectArrayStack* stack = new(ObjectArrayStack);
	
	ASSERT_NO_FATAL_FAILURE(object_array_stack_add(stack, OBJECT(new(IntObject, 24))));
	ASSERT_NO_FATAL_FAILURE(object_array_stack_add(stack, OBJECT(new(IntObject, 56))));
	ASSERT_NO_FATAL_FAILURE(object_array_stack_add(stack, OBJECT(new(IntObject, 87))));
	
	for (int i = 0; i < 100; i ++)
		ASSERT_NO_FATAL_FAILURE(object_array_stack_add(stack, OBJECT(new(IntObject, i))));
	
	EXPECT_EQ(object_array_stack_size(stack), 103);
}

TEST(ObjectArrayStack_functional_testing, not_empty_stack_destructor_testing) {
	// initialize new stack with zero len
	ObjectArrayStack* stack = new(ObjectArrayStack);
	
	object_array_stack_add(stack, OBJECT(new(IntObject, 24)));
	object_array_stack_add(stack, OBJECT(new(IntObject, 56)));
	object_array_stack_add(stack, OBJECT(new(IntObject, 87)));
	
	for (int i = 0; i < 100; i ++)
		object_array_stack_add(stack, OBJECT(new(IntObject, i)));
	
	ASSERT_NO_FATAL_FAILURE(del(stack));
}

TEST(ObjectArrayStack_functional_testing, empty_stack_pop_testing) {
	// initialize new stack with zero len
	ObjectArrayStack* stack = new(ObjectArrayStack);
	
	EXPECT_EQ(object_array_stack_pop(stack), nullptr);
	del(stack);
}

TEST(ObjectArrayStack_functional_testing, not_empty_stack_pop_testing) {
	// initialize new stack with zero len
	ObjectArrayStack* stack = new(ObjectArrayStack);
	object_array_stack_add(stack, OBJECT(new(IntObject, 24)));
	
	EXPECT_EQ(INT_OBJECT(object_array_stack_pop(stack))->val, 24);
}