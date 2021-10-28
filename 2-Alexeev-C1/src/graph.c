#include <stdlib.h>
#include <ctype.h>
#include "graph.h"

struct __graph{
	unsigned int max_index;
	VertexArrayList* vertexses;
};

struct __vertex{
	unsigned int index;
	VertexMarker marker;
	VertexArrayList* neighbours;
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
	vertex_array_list_del_with_nodes(graph->vertexses);
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
		char line[250];
		char* line_p = line;
		
		if(fgets(line, 250, stream) == NULL)
			break;
		
		if (sscanf(line, "%d", &parent_node_index) <= 0)
			break;
		
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
