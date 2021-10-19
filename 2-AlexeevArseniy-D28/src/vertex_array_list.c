#include <stdlib.h>
#include <string.h>
#include "vertex_array_list.h"
#include "graph.h"

#define vertex_swap(a,b){Vertex* tmp = a; a=b; b=tmp;}

#define ALLOC_BLOCK_SIZE 256

struct __vertex{
	unsigned int index;
	
	//for running into graph
	RuntimeVertexMarker run_marker;
	
	//for iteration finding values into graph
	StatusVertexMarker status_marker;
	
	VertexArrayList* neighbours;
	VertexArrayList* hard_link_neighbours;
	unsigned block_counter;
	void* meta_info;
};

Vertex* __vertex_new ( unsigned int vertex_index, void* vertex_meta_info ){
	Vertex* vertex = (Vertex*)malloc(sizeof(Vertex));
	
	if (vertex == NULL)
		return NULL;
	
	vertex->index = vertex_index;
	vertex->run_marker = VERTEX_AVAILABLE;
	vertex->status_marker = VERTEX_FREE;
	vertex->neighbours = vertex_array_list_new(0);
	vertex->hard_link_neighbours = vertex_array_list_new(0);
	vertex->meta_info = vertex_meta_info;
	vertex->block_counter = 0;
	
	if (vertex->neighbours == NULL){
		free(vertex);
		return NULL;
	}
	
	return vertex;
}

void __vertex_del(Vertex* vertex){
	vertex_array_list_del(vertex->neighbours);
	vertex_array_list_del(vertex->hard_link_neighbours);
	free(vertex->meta_info);
	free(vertex);
}

unsigned vertex_get_index(Vertex* vertex){
	return vertex->index;
}

void* vertex_get_meta_info(Vertex* vertex){
	return vertex->meta_info;
}

void vertex_set_status(Vertex* vertex, StatusVertexMarker marker){
	if (vertex == NULL)
		return;
	
	if (marker == VERTEX_IN_USE){
		vertex->status_marker = marker;
		vertex->block_counter++;
	}
	else {
		if (vertex->block_counter > 0)
			vertex->block_counter--;
		
		if (vertex->block_counter == 0)
			vertex->status_marker = marker;
	}
	
	unsigned len = vertex_array_list_len(vertex->hard_link_neighbours);
	for (unsigned i = 0; i < len; i++){
		Vertex* neighbour = vertex_array_list_get(vertex->hard_link_neighbours, i);
		vertex_set_status(neighbour, marker);
	}
}

void vertex_print_info(Vertex* vertex, FILE* stream){
	fprintf(stream, "Vertex %d:\n", vertex->index);
	fprintf(stream, "\t-status marker: %s\n", ((vertex->status_marker == VERTEX_FREE)? "VERTEX_FREE": "VERTEX_IN_USE"));
	fprintf(stream, "\t-neighbours: %d\n", vertex_array_list_len(vertex->neighbours));
	fprintf(stream, "\t-hard link neighbours: %d\n", vertex_array_list_len(vertex->hard_link_neighbours));
	fprintf(stream, "\t-blocks amount: %d\n", vertex->block_counter);
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

void vertex_array_list_vertex_index_sort(VertexArrayList* list, int (*vertex_meta_cmp)(Vertex*, Vertex*)){
	if (vertex_meta_cmp != NULL)
		__quicksort(list->arr, list->len, vertex_meta_cmp);
	else
		__quicksort(list->arr, list->len, __vertex_comparing);
}

void vertex_array_list_comb(VertexArrayList* list, int (*vertex_meta_cmp)(Vertex*, Vertex*)){
	vertex_array_list_vertex_index_sort(list,vertex_meta_cmp);
	for (int i = 0; i + 1 < vertex_array_list_len(list); i++){
		Vertex* vertex1 = vertex_array_list_get(list, i);
		Vertex* vertex2 = vertex_array_list_get(list, i + 1);
		if (__vertex_comparing(vertex1, vertex2) == 0)
			vertex_array_list_pop(list, i--);
	}
}

void vertex_array_list_print_indexses(VertexArrayList* list, FILE* stream, const char* separator){
	unsigned len = list->len;
	
	for (unsigned i = 0; i < len; i++){
		Vertex* vertex = vertex_array_list_get(list, i);
		if (vertex == NULL)
			fprintf(stream, "NULL");
		else
			fprintf(stream, "%d", vertex->index);
		
		if (i < len-1)
			fprintf(stream, "%s", separator);
	}
	putc('\n', stream);
}

