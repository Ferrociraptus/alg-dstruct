#include <stdlib.h>
#include <string.h>
#include "vertex_array_list.h"
#include "graph.h"

#define vertex_swap(a,b){Vertex* tmp = a; a=b; b=tmp;}

#define ALLOC_BLOCK_SIZE 256

struct __vertex{
	unsigned int index;
	VertexMarker marker;
	VertexArrayList* neighbours;
};

Vertex* __vertex_new(unsigned int vertex_index){
	Vertex* vertex = (Vertex*)malloc(sizeof(Vertex));
	
	if (vertex == NULL)
		return NULL;
	
	vertex->index = vertex_index;
	vertex->marker = VERTEX_AVAILABLE;
	vertex->neighbours = vertex_array_list_new(0);
	
	if (vertex->neighbours == NULL){
		free(vertex);
		return NULL;
	}
	
	return vertex;
}

void __vertex_del(Vertex* vertex){
	vertex_array_list_del(vertex->neighbours);
	free(vertex);
}

unsigned vertex_get_index(Vertex* vertex){
	return vertex->index;
}

struct __vertex_array_list{
	unsigned int size; //size of allocated memory
	unsigned int len;  //amount of elements in list
	Vertex** arr;
};

static int __index_to_list_range(VertexArrayList* list, int index){
	if (index < 0)
	index = list->len - index;

	if (index < 0)
		index = 0;
	
	if (index >= list->len && list->len > 0)
		index = list->len - 1;
	
	return index;
}

static void* __vertex_list_realloc_for_elements_amount(VertexArrayList* list, unsigned new_elements_amount){
	unsigned alloc_size = 0;
	unsigned new_size = new_elements_amount * sizeof(Vertex*);
	unsigned alloc_rest = new_size % ALLOC_BLOCK_SIZE;
	alloc_size = new_size - alloc_rest + ((alloc_rest != 0) ? ALLOC_BLOCK_SIZE : 0);
	
	if (list->size != alloc_size){
		Vertex** new_ptr = NULL;
		
		if (alloc_size != 0)
			new_ptr = realloc(list->arr, sizeof(Vertex*)*alloc_size);
		else
			new_ptr = realloc(list->arr, sizeof(Vertex*));
		
		if (new_ptr == NULL)
			return NULL;
		
		list->arr = new_ptr;
		list->size = alloc_size;
	}
	return list->arr;
}

VertexArrayList* vertex_array_list_new(unsigned start_size){
	VertexArrayList* list = (VertexArrayList*)malloc(sizeof(VertexArrayList));
	
	if (list == NULL)
		return NULL;
	
	list->len = start_size;
	list->size = 0;
	list->arr = malloc(sizeof(Vertex*));
	
	   __vertex_list_realloc_for_elements_amount(list, start_size);
	
	if (list->arr == NULL){
		free(list);
		return NULL;
	}
	
	return list;
}


void vertex_array_list_del(VertexArrayList* list){
	free(list->arr);
	free(list);
}

void vertex_array_list_del_with_nodes(VertexArrayList* list){
	for (int i = 0; i < list->len; i++)
		if (list->arr[i] != NULL)
			__vertex_del(list->arr[i]);
}

Vertex* vertex_array_list_get(VertexArrayList* list, int index){
	index = __index_to_list_range(list, index);
	return list->arr[index];
}

void vertex_array_list_set(VertexArrayList* list, int index, Vertex* vertex){
	list->arr[__index_to_list_range(list, index)] = vertex;
}

void vertex_array_list_append(VertexArrayList* list, Vertex* vertex){
	   __vertex_list_realloc_for_elements_amount(list, ++list->len);
	list->arr[list->len - 1] = vertex;
}


void vertex_array_list_insert(VertexArrayList* list, int index, Vertex* vertex){
	index = __index_to_list_range(list, index);
	
	   __vertex_list_realloc_for_elements_amount(list, ++list->len);
	
	memcpy(list->arr+index+1, list->arr+index, sizeof(Vertex*)*(list->len - index - 1));
	
	list->arr[index] = vertex;
}

void vertex_array_list_expand_insert(VertexArrayList* list, int index, Vertex* vertex){
	if (index >= list->len){
		      __vertex_list_realloc_for_elements_amount(list, index + 1);
		for (int i = list->len-1; i <= index; i++)
			list->arr[i] = NULL;
		list->arr[index] = vertex;
		return;
	}
	else
		vertex_array_list_insert(list, index, vertex);
}

Vertex* vertex_array_list_pop(VertexArrayList* list, int index){
	index = __index_to_list_range(list, index);
	
	if (list->len == 0)
		return NULL;
	
	Vertex* poped_vertex = list->arr[index];
	
	memcpy(list->arr + index, list->arr + index + 1, sizeof(Vertex*) * (list->len - index - 1));
	
	list->len--;
	
	   __vertex_list_realloc_for_elements_amount(list, list->len);
	
	return poped_vertex;
}

int vertex_array_list_len(VertexArrayList* list){
	return list->len;
}

static int __vertex_comparing(Vertex* vertex1, Vertex* vertex2){
	if (vertex1->index > vertex2->index)
		return 1;
	else if (vertex1->index < vertex2->index)
		return -1;
	else
		return 0;
}

static void __quicksort(Vertex** array, int size, int (*comparing_fun)(Vertex*, Vertex*)){
	if (size < 2)
		return;
	
	int l = 0;
	int r = size - 2;
	
	Vertex* pivot = array[size/2];
	vertex_swap(array[size/2], array[size-1]);

	while (l <= r){
		while ((*comparing_fun)(array[l], pivot) < 0)
			l++;
		while ((r>=0) && (*comparing_fun)(array[r], pivot) >= 0)
			r--;
		
		if ((l < r) && ((*comparing_fun)(array[l], pivot) >= 0) && ((*comparing_fun)(array[r], pivot) < 0)){
			vertex_swap(array[l], array[r]);
		}
	}
	vertex_swap(array[l], array[size-1]);
	__quicksort(array, l, comparing_fun);
	__quicksort(array + l + 1, size - l - 1, comparing_fun);
}

void vertex_array_list_vertex_index_sort(VertexArrayList* list){
	__quicksort(list->arr, list->len, __vertex_comparing);
}

void vertex_array_list_comb(VertexArrayList* list){
	vertex_array_list_vertex_index_sort(list);
	for (int i = 0; i + 1 < vertex_array_list_len(list); i++){
		Vertex* vertex1 = vertex_array_list_get(list, i);
		Vertex* vertex2 = vertex_array_list_get(list, i + 1);
		if (__vertex_comparing(vertex1, vertex2) == 0)
			vertex_array_list_pop(list, i--);
	}
}
