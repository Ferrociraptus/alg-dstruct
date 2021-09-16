#include "gtest/gtest.h"

extern "C" {
#include <stdlib.h>
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


TEST(ObjectLinkedList_functional_testing, zero_size_list_initialization_testing) {
	// initialize new list with zero len
	ObjectLinkedList* list;
	ASSERT_NO_FATAL_FAILURE((list = new(ObjectLinkedList, 0)));
	
	EXPECT_EQ(object_linked_list_len(list), 0);
}

TEST(ObjectLinkedList_functional_testing, list_destructor_testing) {
	// initialize new list with zero len
	ObjectLinkedList* list = new(ObjectLinkedList, 0);
	ASSERT_NO_FATAL_FAILURE(del(list));
	
	ASSERT_NO_FATAL_FAILURE(list = new(ObjectLinkedList, 3));
	ASSERT_NO_FATAL_FAILURE(del(list));
}

TEST(ObjectLinkedList_functional_testing, list_get_len_testing) {
	// initialize new list with zero len
	ObjectLinkedList* list = new(ObjectLinkedList, 100);
	EXPECT_EQ(LIST_TYPE_DECODE(list)->len, object_linked_list_len(list));
}

TEST(ObjectLinkedList_functional_testing, multy_size_list_empty_initialization_testing) {
	// simple init
	ObjectLinkedList* list;
	
	list = new(ObjectLinkedList, 100);
	EXPECT_EQ(LIST_TYPE_DECODE(list)->len, 100);
	del(list);
}

TEST(ObjectLinkedList_functional_testing, zero_size_list_clear_testing){
	ObjectLinkedList* list = new(ObjectLinkedList, 0);
	EXPECT_NO_FATAL_FAILURE(object_linked_list_clear(list));
	EXPECT_EQ(LIST_TYPE_DECODE(list)->len, 0);
	del(list);
}

TEST(ObjectLinkedList_functional_testing, zero_size_list_get_testing) {
	// initialize new list with zero len
	ObjectLinkedList* list = new(ObjectLinkedList, 0);
	
	EXPECT_EQ(object_linked_list_get(list, 0), nullptr);
	del(list);
}

TEST(ObjectLinkedList_functional_testing, single_size_list_set_testing) {
	// initialize new list with zero len
	ObjectLinkedList* list = new(ObjectLinkedList, 1);
	
	EXPECT_NO_FATAL_FAILURE(object_linked_list_set(list, 0, OBJECT(new(IntObject, 1))));
	
	EXPECT_EQ(INT_OBJECT(LIST_TYPE_DECODE(list)->current_node->item)->val, 1);
	del(list);
}

TEST(ObjectLinkedList_functional_testing, zero_size_list_append_testing) {
	// initialize new list with zero len
	ObjectLinkedList* list = new(ObjectLinkedList, 0);
	EXPECT_NO_FATAL_FAILURE(object_linked_list_append(list, OBJECT(new(IntObject, 1))));
	
	EXPECT_EQ(INT_OBJECT(LIST_TYPE_DECODE(list)->current_node->item)->val, 1);
	
	EXPECT_EQ(LIST_TYPE_DECODE(list)->len, 1);

	del(list);
}

TEST(ObjectLinkedList_functional_testing, zero_size_list_insert_testing) {
	// initialize new list with zero len
	ObjectLinkedList* list = new(ObjectLinkedList, 0);
	
	EXPECT_NO_FATAL_FAILURE(object_linked_list_insert(list, 0, OBJECT(new(IntObject, 1))));
	EXPECT_EQ(INT_OBJECT(LIST_TYPE_DECODE(list)->current_node->item)->val, 1);
	
	object_linked_list_clear(list);
	EXPECT_NO_FATAL_FAILURE(object_linked_list_insert(list, 10, OBJECT(new(IntObject, 2))));
	EXPECT_EQ(INT_OBJECT(LIST_TYPE_DECODE(list)->current_node->item)->val, 2);
	
	del(list);
}

TEST(ObjectLinkedList_functional_testing, zero_size_list_pop_testing) {
	ObjectLinkedList* list = new(ObjectLinkedList, 0);
	EXPECT_EQ(object_linked_list_pop(list, 0), nullptr);
	del(list);
}

TEST(ObjectLinkedList_functional_testing, zero_size_list_remove_testing) {
	ObjectLinkedList* list = new(ObjectLinkedList, 0);
	EXPECT_NO_FATAL_FAILURE(object_linked_list_remove(list, 0));
	del(list);
}
