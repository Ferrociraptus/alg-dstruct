#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include "vertex_array_list.h"

#define META_CMP_FUN(fun) ((int(*)(void*,void*))fun)

typedef struct __graph Graph;

typedef enum __graph_status {GRAPH_OK,
	GRAPH_TRUE,
	GRAPH_FALSE,
	GRAPH_ERR,
	GRAPH_VERTEX_DOES_NOT_EXIST} graph_status;

Graph* graph_new(unsigned max_index);
void graph_del(Graph* graph);
void graph_init_all_vertex(Graph* graph);
void graph_reset(Graph* graph);

void graph_add_vertex(Graph* graph,
						unsigned vertex_index,
						void* vertex_meta);

void graph_create_biderectional_edge(Graph* graph,
						unsigned int first_vertex_index,
						unsigned int second_vertex_index);

void graph_create_one_derectional_edge(Graph* graph,
						unsigned int from_vertex_index,
						unsigned int to_vertex_index);

void graph_create_one_derectional_hard_link_edge(Graph* graph,
						unsigned int from_vertex_index,
						unsigned int to_vertex_index);

VertexArrayList* graph_pre_order_traversal(Graph* graph,
						unsigned int start_vertex_index,
						int (*vertex_meta_cmp)(Vertex*, Vertex*));

Graph* graph_parse_graph_adjacency_list(FILE* stream);

graph_status graph_is_vertex_joined(Graph* graph,
						int vertex_index1,
						int vertex_index2);

/* 
 Addiction for graph_find_non_recurring_vertex function parameter:
 
 vertex_meta_cmp(void* meta1, void* meta2) - function for comparing.
 It used to find nesessary value
 
 meta1 - current meta value (it can be NULL)
 meta2 - comparabale meta value (wich one we want to find) (it can be NULL)
*/
Vertex* graph_find_vertex(Graph* graph,
						int start_index,
						int(*vertex_meta_cmp)(void* meta1, void* meta2));

Vertex* graph_find_non_recurring_vertex(Graph* graph,
						int start_index,
						int(*vertex_meta_cmp)(void* meta1, void* meta2));


/* 
 Addiction for graph_find_specific_path function parameter:
 
 check_specification_fun(VertexArrayList* ans) - function for check
 result list specification. It used to check list for specific rule. 
 
 ans - list whic one should be checked
*/
VertexArrayList* graph_find_specific_path(Graph* graph,
						unsigned start_index,
						unsigned min_path_len,
						int (*check_specification_fun)(VertexArrayList* ans));

#endif
