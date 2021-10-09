
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//------------------------VertexArrayList header----------------------
#ifndef VERTEX_ARRAY_LIST_H
#define VERTEX_ARRAY_LIST_H

typedef enum{VERTEX_AVAILABLE, VERTEX_PASSED, VERTEX_MARKER} VertexMarker;

typedef struct __vertex Vertex;
typedef struct __vertex_array_list VertexArrayList;

Vertex* __vertex_new(unsigned int vertex_index);
void __vertex_del(Vertex* vertex);
unsigned vertex_get_index(Vertex* vertex);


VertexArrayList* vertex_array_list_new(unsigned start_size);
void vertex_array_list_del(VertexArrayList* list);
Vertex* vertex_array_list_get(VertexArrayList* list, int index);
void vertex_array_list_set(VertexArrayList* list, int index, Vertex* vertex);
void vertex_array_list_append(VertexArrayList* list, Vertex* vertex);
void vertex_array_list_insert(VertexArrayList* list, int index, Vertex* vertex);
void vertex_array_list_expand_insert(VertexArrayList* list, int index, Vertex* vertex);
Vertex* vertex_array_list_pop(VertexArrayList* list, int index);
int vertex_array_list_len(VertexArrayList* list);
void vertex_array_list_vertex_index_sort(VertexArrayList* list);
void vertex_array_list_comb(VertexArrayList* list);

#endif
//---------------------------------end--------------------------------
//--------------------------------------------------------------------



//------------------------VertexArrayList source----------------------
#define vertex_swap(a,b){Vertex* tmp = a; a=b; b=tmp;}

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
	unsigned int len;
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

VertexArrayList* vertex_array_list_new(unsigned start_size){
	VertexArrayList* list = (VertexArrayList*)malloc(sizeof(VertexArrayList));
	
	if (list == NULL)
		return NULL;
	
	list->len = start_size;
	if (start_size == 0)
		list->arr = (Vertex**)malloc(sizeof(Vertex*));
	else
		list->arr = (Vertex**)malloc(sizeof(Vertex*) * start_size);
	
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


Vertex* vertex_array_list_get(VertexArrayList* list, int index){
	index = __index_to_list_range(list, index);
	return list->arr[index];
}

void vertex_array_list_set(VertexArrayList* list, int index, Vertex* vertex){
	list->arr[__index_to_list_range(list, index)] = vertex;
}

void vertex_array_list_append(VertexArrayList* list, Vertex* vertex){
	list->arr = (Vertex**)realloc(list->arr, sizeof(Vertex*) * ++list->len);
	list->arr[list->len - 1] = vertex;
}


void vertex_array_list_insert(VertexArrayList* list, int index, Vertex* vertex){
	index = __index_to_list_range(list, index);
	
	list->arr = (Vertex**)realloc(list->arr, sizeof(Vertex*) * ++list->len);
	
	memcpy(list->arr+index+1, list->arr+index, sizeof(Vertex*)*(list->len - index - 1));
	
	list->arr[index] = vertex;
}

void vertex_array_list_expand_insert(VertexArrayList* list, int index, Vertex* vertex){
	if (index >= list->len){
		list->arr = (Vertex**)realloc(list->arr, sizeof(Vertex*) * (index + 1));
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
	if (list->len == 0)
		list->arr = (Vertex**)realloc(list->arr, sizeof(Vertex*));
	else
		list->arr = (Vertex**)realloc(list->arr, sizeof(Vertex*) * list->len);
	
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
//---------------------------------end--------------------------------
//--------------------------------------------------------------------



//----------------------------Graph header----------------------------
#ifndef GRAPH_H
#define GRAPH_H

typedef struct __graph Graph;

Graph* graph_new(unsigned max_index);
void graph_del(Graph* graph);
void graph_init_all_vertex(Graph* graph);
void graph_add_vertex(Graph* graph, unsigned vertex_index);

void graph_create_biderectional_edge(Graph* graph,
									 unsigned int first_vertex_index,
									 unsigned int second_vertex_index);

void graph_create_one_derectional_edge(Graph* graph,
									   unsigned int from_vertex_index,
									   unsigned int to_vertex_index);

VertexArrayList* graph_pre_order_traversal(Graph* graph, unsigned int start_vertex_index);

Graph* graph_parse_graph(FILE* stream);

#endif
//---------------------------------end--------------------------------
//--------------------------------------------------------------------



//----------------------------Graph source----------------------------


struct __graph{
	unsigned int max_index;
	VertexArrayList* vertexses;
};

Graph* graph_new(unsigned max_index){
	Graph* graph = (Graph*)malloc(sizeof(Graph));
	if (graph == NULL)
		return NULL;
	
	graph->max_index = max_index;
	graph->vertexses = vertex_array_list_new(max_index+1);
	for (int i = 0; i <= max_index; i++)
		vertex_array_list_set(graph->vertexses, i, NULL);
	
	if (graph->vertexses == NULL){
		free(graph);
		return NULL;
	}
	return graph;
}

void graph_del(Graph* graph){
	for (int i = 0; i < graph->max_index; i++){
		Vertex* vertex = vertex_array_list_get(graph->vertexses, i);
		if (vertex != NULL)
			__vertex_del (vertex);
	}
	free(graph->vertexses);
	free(graph);
}

void graph_init_all_vertex(Graph* graph){
	for (int i = 0; i < vertex_array_list_len(graph->vertexses); i++)
		vertex_array_list_set(graph->vertexses, i, __vertex_new(i));
}

void graph_add_vertex(Graph* graph, unsigned vertex_index){
	vertex_array_list_expand_insert(graph->vertexses, vertex_index, __vertex_new(vertex_index));
}

void graph_create_biderectional_edge(Graph* graph, unsigned int first_vertex_index, unsigned int second_vertex_index){
	vertex_array_list_append(vertex_array_list_get(graph->vertexses, first_vertex_index)->neighbours, vertex_array_list_get(graph->vertexses, second_vertex_index));
	
	vertex_array_list_append(vertex_array_list_get(graph->vertexses, second_vertex_index)->neighbours, vertex_array_list_get(graph->vertexses, first_vertex_index));
}

void graph_create_one_derectional_edge(Graph* graph, unsigned int from_vertex_index, unsigned int to_vertex_index)
{
	vertex_array_list_append(vertex_array_list_get(graph->vertexses, from_vertex_index)->neighbours, vertex_array_list_get(graph->vertexses, to_vertex_index));
}

void __pre_order_traversal(Vertex* vertex, VertexArrayList* answer_list){
	Vertex* neighbour;
	
	vertex->marker = VERTEX_PASSED;
	
	vertex_array_list_append(answer_list, vertex);
	vertex_array_list_comb(vertex->neighbours);
	
	for (int i = 0; i < vertex_array_list_len(vertex->neighbours); i++){
		neighbour = vertex_array_list_get(vertex->neighbours, i);
		
		if (neighbour->marker == VERTEX_AVAILABLE)
			__pre_order_traversal(neighbour, answer_list);
	}
}

VertexArrayList* graph_pre_order_traversal(Graph* graph, unsigned int start_vertex_index){
	
	VertexArrayList* traversal_ans = vertex_array_list_new(0);
	Vertex* start_node = vertex_array_list_get(graph->vertexses, start_vertex_index);
	
	__pre_order_traversal(start_node, traversal_ans);
	
	return traversal_ans;
}

Graph* graph_parse_graph_adjacency_list(FILE* stream){
	int val;
	fscanf(stream, "%d\n", &val);
	
	Graph* graph = graph_new(val);
	
	if (graph == NULL)
		return NULL;
	
	graph_init_all_vertex(graph);
	
	while (1){
		int parent_node_index, neighbour_node_index;
		int scan_result = 0;
		char line[250];
		char* line_p = line;
		
		if(fgets(line, 250, stream) == NULL)
			break;
		
		sscanf(line, "%d", &parent_node_index);
		
		while(isdigit(*line_p)) line_p++;
		while(*line_p == ' ') line_p++;
		
		while (sscanf(line_p, "%d", &neighbour_node_index) > 0){
			while(isdigit(*line_p)) line_p++;
			while(*line_p == ' ') line_p++;
			graph_create_biderectional_edge(graph, parent_node_index, neighbour_node_index);
		}
	}
	return graph;
}
//---------------------------------end--------------------------------
//--------------------------------------------------------------------

int main(){
	Graph* graph = graph_parse_graph_adjacency_list(stdin);
	VertexArrayList* ans = graph_pre_order_traversal(graph, 0);
	for (int i = 0; i < vertex_array_list_len(ans) - 1; i++)
		printf("%d ", vertex_get_index(vertex_array_list_get(ans, i)));
	printf("%d", vertex_get_index(vertex_array_list_get(ans, vertex_array_list_len(ans) - 1)));
	return 0;
}
