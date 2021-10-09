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
void vertex_array_list_del_with_nodes(VertexArrayList* list);
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
