#include "gtest/gtest.h"

extern "C" {
#include <stdlib.h>
#include "ObjectLinkedStack.h"
#include "ObjectLinkedList.h"
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
#define STACK_TYPE_DECODE(val) ((struct test_stack_type_decode*)(val))
#define LIST_TYPE_DECODE(val) ((struct test_list_type_decode*)(val))
#define NODE_TYPE_DECODE(val) ((struct test_node_type_decode*)(val))

struct test_node_type_decode {
	struct _object_linked_list_node* next_node;
	struct _object_linked_list_node* previous_node;
	Object* item;
};

struct test_list_type_decode {
	Object obj;
	unsigned long len;
	unsigned long current_index;
	struct test_node_type_decode* current_node;
};

struct test_stack_type_decode{
	Object obj;
	struct test_list_type_decode* list;
};


TEST(ObjectLinkedStack_functional_testing, stack_initialization_testing) {
	// initialize new stack with zero len
	ObjectLinkedStack* stack;
	ASSERT_NO_FATAL_FAILURE(stack = new(ObjectLinkedStack));
	
	ASSERT_EQ(STACK_TYPE_DECODE(stack)->list->len, 0);
}


TEST(ObjectLinkedStack_functional_testing, empty_stack_destructor_testing) {
	// initialize new stack with zero len
	ObjectLinkedStack* stack = new(ObjectLinkedStack);
	
	ASSERT_NO_FATAL_FAILURE(del(stack));
}

TEST(ObjectLinkedStack_functional_testing, stack_get_size_testing) {
	// initialize new stack with zero len
	ObjectLinkedStack* stack = new(ObjectLinkedStack);
	
	ASSERT_EQ(STACK_TYPE_DECODE(stack)->list->len, object_linked_stack_size(stack));
}

TEST(ObjectLinkedStack_functional_testing, single_adding_item_to_stack_testing) {
	// initialize new stack with zero len
	ObjectLinkedStack* stack = new(ObjectLinkedStack);
	
	ASSERT_NO_FATAL_FAILURE(object_linked_stack_add(stack, OBJECT(new(IntObject, 1))));
	EXPECT_EQ(STACK_TYPE_DECODE(stack)->list->len, 1);
}

TEST(ObjectLinkedStack_functional_testing, plural_adding_item_to_stack_testing) {
	// initialize new stack with zero len
	ObjectLinkedStack* stack = new(ObjectLinkedStack);
	
	ASSERT_NO_FATAL_FAILURE(object_linked_stack_add(stack, OBJECT(new(IntObject, 1))));
	ASSERT_NO_FATAL_FAILURE(object_linked_stack_add(stack, OBJECT(new(IntObject, 2))));
	ASSERT_NO_FATAL_FAILURE(object_linked_stack_add(stack, OBJECT(new(IntObject, 3))));
	
	EXPECT_EQ(STACK_TYPE_DECODE(stack)->list->len, 3);
}

TEST(ObjectLinkedStack_functional_testing, not_empty_stack_destructor_testing) {
	// initialize new stack with zero len
	ObjectLinkedStack* stack = new(ObjectLinkedStack);
	
	struct test_list_type_decode** stack_list = &(STACK_TYPE_DECODE(stack)->list);
	*stack_list = LIST_TYPE_DECODE(new(ObjectLinkedList, 10));
	
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
	
	struct test_list_type_decode** stack_list = &(STACK_TYPE_DECODE(stack)->list);
	*stack_list = LIST_TYPE_DECODE(new(ObjectLinkedList, 1));
	(*stack_list)->current_node->item = OBJECT(new(IntObject, 1));
	
	EXPECT_EQ(INT_OBJECT(object_linked_stack_pop(stack))->val, 1);
}
