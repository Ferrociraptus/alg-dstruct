#ifndef VOID_LINKED_LIST_H
#define VOID_LINKED_LIST_H

#include "Object.h"

#define OBJECT_LINKED_LIST(obj) ((ObjectLinkedList*)(obj))

typedef struct _object_linked_list ObjectLinkedList;

MEMORY_ALLOCATOR(ObjectLinkedList);
CONSTRUCTOR(ObjectLinkedList, int nodes_amount);

void object_linked_list_clear(ObjectLinkedList* list);
int object_linked_list_insert(ObjectLinkedList* list, int index, Object* item);
int object_linked_list_append(ObjectLinkedList* list, Object* item);
Object* object_linked_list_get(ObjectLinkedList* list, int index);
void object_linked_list_set(ObjectLinkedList* list, int index, Object* item);
void object_linked_list_remove(ObjectLinkedList* list, int index);
Object* object_linked_list_pop(ObjectLinkedList* list, int index);

unsigned long object_linked_list_len(ObjectLinkedList* list);

#endif
