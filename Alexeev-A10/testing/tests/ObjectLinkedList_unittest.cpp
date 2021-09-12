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




TEST(ObjectLinkedList_functional_testing, zero_size_list_initialization_testing) {
	// initialize new list with zero len
	ObjectLinkedList* list = new(ObjectLinkedList, 0);
	
	EXPECT_EQ(object_linked_list_len(list), 0);
}

TEST(ObjectLinkedList_functional_testing, list_destructor_testing) {
	// initialize new list with zero len
	ObjectLinkedList* list = new(ObjectLinkedList, 0);
	ASSERT_NO_FATAL_FAILURE(del(list));
	
	ASSERT_NO_FATAL_FAILURE(list = new(ObjectLinkedList, 3));
	ASSERT_NO_FATAL_FAILURE(del(list));
}

TEST(ObjectLinkedList_functional_testing, multy_size_list_empty_initialization_testing) {
	// simple init
	ObjectLinkedList* list;
	
	list = new(ObjectLinkedList, 0);
	EXPECT_EQ(object_linked_list_len(list), 0);
	del(list);
		
	list = new(ObjectLinkedList, 10);
	EXPECT_EQ(object_linked_list_len(list), 10);
	del(list);
		
	list = new(ObjectLinkedList, 33);
	EXPECT_EQ(object_linked_list_len(list), 33);
	del(list);
	
	for (int i = 0; i < 1000; i++){
		ObjectLinkedList* list = new(ObjectLinkedList, i);
		EXPECT_EQ(object_linked_list_len(list), i);
		del(list);
	}
}

TEST(ObjectLinkedList_functional_testing, zero_size_list_clear_testing){
	ObjectLinkedList* list = new(ObjectLinkedList, 0);
	EXPECT_NO_FATAL_FAILURE(object_linked_list_clear(list));
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
	
	EXPECT_EQ(object_linked_list_len(list), 1);
	
	EXPECT_EQ(object_linked_list_get(list, 0), nullptr);
	del(list);
}

TEST(ObjectLinkedList_functional_testing, zero_size_list_append_testing) {
	// initialize new list with zero len
	ObjectLinkedList* list = new(ObjectLinkedList, 0);
	EXPECT_NO_FATAL_FAILURE(object_linked_list_append(list, OBJECT(new(IntObject, 23))));
	EXPECT_EQ(INT_OBJECT(object_linked_list_get(list, 0))->val, 23);
	EXPECT_EQ(object_linked_list_len(list), 1);

	del(list);
}

TEST(ObjectLinkedList_functional_testing, zero_size_list_insert_testing) {
	// initialize new list with zero len
	ObjectLinkedList* list = new(ObjectLinkedList, 0);
	
	EXPECT_NO_FATAL_FAILURE(object_linked_list_insert(list, 0, OBJECT(new(IntObject, 23))));
	EXPECT_EQ(INT_OBJECT(object_linked_list_get(list, 0))->val, 23);
	
	object_linked_list_clear(list);
	EXPECT_NO_FATAL_FAILURE(object_linked_list_insert(list, 10, OBJECT(new(IntObject, 55))));
	EXPECT_EQ(INT_OBJECT(object_linked_list_get(list, 0))->val, 55);
	

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

TEST(ObjectLinkedList_functional_testing, list_remove_testing) {
	ObjectLinkedList* list = new(ObjectLinkedList, 3);
	
	EXPECT_NO_FATAL_FAILURE(object_linked_list_set(list, 0, OBJECT(new(IntObject, 11))));
	EXPECT_NO_FATAL_FAILURE(object_linked_list_set(list, 1, OBJECT(new(IntObject, 22))));
	EXPECT_NO_FATAL_FAILURE(object_linked_list_set(list, 2, OBJECT(new(IntObject, 33))));
	
	EXPECT_NO_FATAL_FAILURE(object_linked_list_remove(list, 1));
	EXPECT_EQ(object_linked_list_len(list), 2);
	
	EXPECT_EQ(INT_OBJECT(object_linked_list_get(list, 0))->val, 11);
	EXPECT_EQ(INT_OBJECT(object_linked_list_get(list, 1))->val, 33);
	
	del(list);
}

TEST(ObjectLinkedList_functional_testing, list_pop_testing) {
	ObjectLinkedList* list = new(ObjectLinkedList, 3);
	
	EXPECT_NO_FATAL_FAILURE(object_linked_list_set(list, 0, OBJECT(new(IntObject, 11))));
	EXPECT_NO_FATAL_FAILURE(object_linked_list_set(list, 1, OBJECT(new(IntObject, 22))));
	EXPECT_NO_FATAL_FAILURE(object_linked_list_set(list, 2, OBJECT(new(IntObject, 33))));
	
	EXPECT_EQ(INT_OBJECT(object_linked_list_pop(list, 1))->val, 22);
	EXPECT_EQ(object_linked_list_len(list), 2);
	
	EXPECT_EQ(INT_OBJECT(object_linked_list_get(list, 0))->val, 11);
	EXPECT_EQ(INT_OBJECT(object_linked_list_get(list, 1))->val, 33);
	
	del(list);
}
