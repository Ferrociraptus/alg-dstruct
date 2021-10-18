#ifndef VERTEX_ARRAY_LIST_H
#define VERTEX_ARRAY_LIST_H
#include <stdio.h>

typedef enum{VERTEX_AVAILABLE, VERTEX_PASSED} RuntimeVertexMarker;
typedef enum{VERTEX_FREE, VERTEX_IN_USE} StatusVertexMarker;

typedef struct __vertex Vertex;
typedef struct __vertex_array_list VertexArrayList;

Vertex* __vertex_new(unsigned int vertex_index, void* vertex_meta_info);
void __vertex_del(Vertex* vertex);
unsigned vertex_get_index(Vertex* vertex);
void* vertex_get_meta_info(Vertex* vertex);
void vertex_set_status(Vertex* vertex, StatusVertexMarker marker);
void vertex_print_info(Vertex* vertex, FILE* stream);

VertexArrayList* vertex_array_list_new(unsigned start_size);
void vertex_array_list_del(VertexArrayList* list);
void vertex_array_list_del_with_nodes(VertexArrayList* list);
Vertex* vertex_array_list_get(VertexArrayList* list, int index);
void vertex_array_list_set(VertexArrayList* list, int index, Vertex* vertex);
void vertex_array_list_append(VertexArrayList* list, Vertex* vertex);
void vertex_array_list_insert(VertexArrayList* list, int index, Vertex* vertex);
void vertex_array_list_expand_insert(VertexArrayList* list, int index, Vertex* vertex);
Vertex* vertex_array_list_pop(VertexArrayList* list, int index);
int vertex_array_list_len(VertexArrayList* list);

void vertex_array_list_vertex_index_sort(VertexArrayList* list,
							int (*vertex_meta_cmp)(Vertex*, Vertex*));

void vertex_array_list_comb(VertexArrayList* list,
							int (*vertex_meta_cmp)(Vertex*, Vertex*));

void vertex_array_list_print_indexses(VertexArrayList* list, FILE* stream, const char* separator);

#endif
