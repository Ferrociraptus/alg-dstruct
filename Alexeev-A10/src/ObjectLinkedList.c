#include <stdlib.h>
#include "ObjectLinkedList.h"

#ifndef abs
#define abs(A) (((A) < 0) ? -(A) : (A))
#endif

typedef struct _object_linked_list_node {
	struct _object_linked_list_node* next_node;
	struct _object_linked_list_node* previous_node;
	Object* item;
} ObjectLinkedListNode;

static ObjectLinkedListNode* __object_linked_list_node_new(Object* item) {
	ObjectLinkedListNode* node = (ObjectLinkedListNode*) malloc(sizeof(ObjectLinkedListNode));

	if (node == NULL)
		return NULL;

	node->item = item;
	return node;
}

inline static void __object_linked_list_node_destroy_with_value(ObjectLinkedListNode* node) {
	if (node == NULL)
		return;
	del(node->item);
	free(node);
}

inline static void __object_linked_list_node_destroy_without_value(ObjectLinkedListNode* node) {
	if (node == NULL)
		return;
	free(node);
}

struct _object_linked_list {
	Object obj;
	unsigned long len;
	unsigned long current_index;
	ObjectLinkedListNode* current_node;
	Destructor item_destructor;
};

static void __go_to_index(ObjectLinkedList* list, long long index) {
	
	if (index == list->current_index)
		return;

	if (index > list->len)
		index = list->len - 1;

	if (index < 0)
		index = 0;

	int diff1 = list->current_index - index;
	int diff2 = -index - list->len + list->current_index;
	int diff = (abs(diff1) < abs(diff2)) ? diff1 : diff2;
	list->current_index = index;

	for (int i = 0; i < abs(diff); i++)
		if (diff < 0)
			list->current_node = list->current_node->next_node;
		else
			list->current_node = list->current_node->previous_node;
}

static ObjectLinkedListNode* __extract_node_by_index(ObjectLinkedList* list, int index){
	__go_to_index(list, index);
	ObjectLinkedListNode* extract_node = list->current_node;
	
	if (extract_node == NULL)
		return NULL;

	list->len--;
	extract_node->previous_node->next_node = extract_node->next_node;
	extract_node->next_node->previous_node = extract_node->previous_node;

	if (list->len > 0) {
		list->current_node = extract_node->next_node;
		list->current_index = list->current_index % list->len;
	}
	else {
		list->current_node = NULL;
		list->current_index = 0;
	}
	return extract_node;
}

static DESTRUCTOR(ObjectLinkedList){
	if (self == NULL)
		return NULL;

	object_linked_list_clear(self);
	return self;
}

DEFAULT_MEMORY_ALLOCATOR(ObjectLinkedList);

CONSTRUCTOR(ObjectLinkedList, int nodes_amount) {
	if (self == NULL)
		return NULL;
	REGISTER_DESTRUCTOR(self, DESTRUCTOR_FUN(ObjectLinkedList));
	self->len = 0;
	self->current_index = 0;
	self->current_node = NULL;

	ObjectLinkedListNode* node;

	if (nodes_amount > 0) {
		node = __object_linked_list_node_new(NULL);

		if (node == NULL)
			return NULL;

		self->current_node = node;
		node->previous_node = node;
		node->next_node = node;
		self->len++;

		for (int i = 0; i < nodes_amount - 1; i++) {
			node->next_node = __object_linked_list_node_new(NULL);

			if (node->next_node == NULL) {
				node->next_node = self->current_node;
				del(OBJECT(self));
				return NULL;
			}

			node->next_node->previous_node = node;
			node = node->next_node;
			self->len++;
		}

		node->next_node = self->current_node;
		self->current_node->previous_node = node;
	}

	return self;
};

void object_linked_list_clear(ObjectLinkedList* list) {
	ObjectLinkedListNode* del_node = list->current_node;
	for (int i = 0; i < list->len; i++){
		list->current_node = del_node->next_node;
		__object_linked_list_node_destroy_with_value(del_node);
		del_node = list->current_node;
	}
	
	list->len = 0;
	list->current_index = 0;
	list->current_node = NULL;
}

int object_linked_list_insert(ObjectLinkedList* list, int index, Object* item) {
	ObjectLinkedListNode* new_node = __object_linked_list_node_new(item);

	if (new_node == NULL || index < 0)
		return -1;

	if (index > list->len)
		index = list->len;

	if (list->len == 0) {
		new_node->next_node = new_node;
		new_node->previous_node = new_node;
		list->current_node = new_node;
	}
	else {
		__go_to_index(list, index);
		new_node->next_node = list->current_node;
		new_node->previous_node = list->current_node->previous_node;
		new_node->previous_node->next_node = new_node;
		list->current_node->previous_node = new_node;
	}

	list->current_index++;
	list->len++;
	return 1;
}

int object_linked_list_append(ObjectLinkedList* list, Object* item) {
	return object_linked_list_insert(list, list->len, item);
}

Object* object_linked_list_get(ObjectLinkedList* list, int index) {
	__go_to_index(list, index);
	if (list->current_node != NULL)
		return list->current_node->item;
	return NULL;
}

void object_linked_list_set(ObjectLinkedList* list, int index, Object* item) {
	__go_to_index(list, index);

	if (list->current_node->item != NULL)
		del(list->current_node->item);

	list->current_node->item = item;
}

void object_linked_list_remove(ObjectLinkedList* list, int index) {
	ObjectLinkedListNode* node = __extract_node_by_index(list, index);
	__object_linked_list_node_destroy_with_value(node);
}

Object* object_linked_list_pop(ObjectLinkedList* list, int index) {
	Object* value;
	ObjectLinkedListNode* node = __extract_node_by_index(list, index);
	if (node != NULL)
		value = node->item;
	else
		value = NULL;
	__object_linked_list_node_destroy_without_value(node);
	return value;
}

unsigned long object_linked_list_len(ObjectLinkedList* list) {
	return list->len;
}


