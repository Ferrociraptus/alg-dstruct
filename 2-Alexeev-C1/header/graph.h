#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include "vertex_array_list.h"

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

Graph* graph_parse_graph_adjacency_list(FILE* stream);

#endif
