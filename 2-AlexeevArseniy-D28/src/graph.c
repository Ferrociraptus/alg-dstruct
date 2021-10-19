#include <stdlib.h>
#include <ctype.h>
#include "graph.h"

struct __graph{
	VertexArrayList* vertexses;
	unsigned int max_index;
};

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
		vertex_array_list_set(graph->vertexses, i, __vertex_new(i, NULL));
}

void graph_reset(Graph* graph){
	unsigned list_len = vertex_array_list_len(graph->vertexses);
	for (unsigned i = 0; i < list_len; i++){
		Vertex* vertex = vertex_array_list_get(graph->vertexses, i);
		if (vertex != NULL){
			vertex->run_marker = VERTEX_AVAILABLE;
			vertex->status_marker = VERTEX_FREE;
		}
	}
}

void __graph_drop_runing(Graph* graph){
	unsigned list_len = vertex_array_list_len(graph->vertexses);
	for (unsigned i = 0; i < list_len; i++){
		Vertex* vertex = vertex_array_list_get(graph->vertexses, i);
		if (vertex != NULL)
			vertex->run_marker = VERTEX_AVAILABLE;
	}
}

void graph_add_vertex(Graph* graph, unsigned vertex_index, void* vertex_meta){
	if (vertex_array_list_len(graph->vertexses) <= vertex_index)
		vertex_array_list_expand_insert(graph->vertexses, vertex_index, __vertex_new(vertex_index, vertex_meta));
	else if (vertex_array_list_get(graph->vertexses, vertex_index) == NULL)
		vertex_array_list_set(graph->vertexses, vertex_index, __vertex_new(vertex_index, vertex_meta));
}

void graph_create_biderectional_edge(Graph* graph, unsigned int first_vertex_index, unsigned int second_vertex_index){
	vertex_array_list_append(vertex_array_list_get(graph->vertexses, first_vertex_index)->neighbours, vertex_array_list_get(graph->vertexses, second_vertex_index));
	
	vertex_array_list_append(vertex_array_list_get(graph->vertexses, second_vertex_index)->neighbours, vertex_array_list_get(graph->vertexses, first_vertex_index));
}

void graph_create_one_derectional_edge(Graph* graph, unsigned int from_vertex_index, unsigned int to_vertex_index)
{
	vertex_array_list_append(vertex_array_list_get(graph->vertexses, from_vertex_index)->neighbours, vertex_array_list_get(graph->vertexses, to_vertex_index));
}

void graph_create_one_derectional_hard_link_edge(Graph* graph,
						unsigned int from_vertex_index,
						unsigned int to_vertex_index){
	Vertex* vertex_from = vertex_array_list_get(graph->vertexses, from_vertex_index);
	Vertex* vertex_to = vertex_array_list_get(graph->vertexses, to_vertex_index);
	
	vertex_array_list_append(vertex_from->neighbours, vertex_to);
	vertex_array_list_append(vertex_to->hard_link_neighbours, vertex_from);
}

void __pre_order_traversal(Vertex* vertex, VertexArrayList* answer_list, int (*vertex_meta_cmp)(Vertex*, Vertex*)){
	Vertex* neighbour;
	
	vertex->run_marker = VERTEX_PASSED;
	
	vertex_array_list_append(answer_list, vertex);
	vertex_array_list_comb(vertex->neighbours, NULL);
	
	for (int i = 0; i < vertex_array_list_len(vertex->neighbours); i++){
		neighbour = vertex_array_list_get(vertex->neighbours, i);
		
		if (neighbour->run_marker == VERTEX_AVAILABLE)
			__pre_order_traversal(neighbour, answer_list, vertex_meta_cmp);
	}
}

VertexArrayList* graph_pre_order_traversal(Graph* graph, unsigned int start_vertex_index, int (*vertex_meta_cmp)(Vertex*, Vertex*)){
	
	VertexArrayList* traversal_ans = vertex_array_list_new(0);
	Vertex* start_node = vertex_array_list_get(graph->vertexses, start_vertex_index);
	
	__pre_order_traversal(start_node, traversal_ans, vertex_meta_cmp);
	
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

graph_status graph_is_vertex_joined(Graph* graph, int vertex_index1, int vertex_index2){
	Vertex* vertex1, *vertex2;
	
	if ((vertex1 = vertex_array_list_get(graph->vertexses, vertex_index1)) == NULL)
		return GRAPH_VERTEX_DOES_NOT_EXIST;
	if ((vertex2 = vertex_array_list_get(graph->vertexses, vertex_index2)) == NULL)
		return GRAPH_VERTEX_DOES_NOT_EXIST;
	
	unsigned list_len = vertex_array_list_len(vertex1->neighbours);
	for (unsigned i = 0; i < list_len; i++)
		if (vertex2 == vertex_array_list_get(vertex1->neighbours,i))
			return GRAPH_TRUE;
		
	list_len = vertex_array_list_len(vertex2->neighbours);
	for (unsigned i = 0; i < list_len; i++)
		if (vertex1 == vertex_array_list_get(vertex2->neighbours,i))
			return GRAPH_TRUE;
		
	return GRAPH_FALSE;
}

void __pre_order_traversal_search(Vertex* vertex,
			int (*vertex_meta_cmp)(void*, void*),
			Vertex** search_result){
	
	Vertex* neighbour;
	
	vertex->run_marker = VERTEX_PASSED;
	
	for (int i = 0; i < vertex_array_list_len(vertex->neighbours); i++){
		neighbour = vertex_array_list_get(vertex->neighbours, i);
		if (vertex_meta_cmp(*search_result, neighbour->meta_info))
			*search_result = neighbour->meta_info;
			
		
		if (neighbour->run_marker == VERTEX_AVAILABLE)
			__pre_order_traversal_search(neighbour,
						vertex_meta_cmp,
						search_result);
	}
}

void __pre_order_traversal_non_recurring_search(Vertex* vertex,
			int (*vertex_meta_cmp)(void*, void*),
			Vertex** search_result){
	
	Vertex* neighbour;
	
	vertex->run_marker = VERTEX_PASSED;
	
	int len = vertex_array_list_len(vertex->neighbours);
	for (int i = 0; i < len; i++){
		neighbour = vertex_array_list_get(vertex->neighbours, i);
		
		int cmp_result = 0;
		if (*search_result != NULL)
			cmp_result = (*vertex_meta_cmp)((*search_result)->meta_info, neighbour->meta_info);
		else
			cmp_result = (*vertex_meta_cmp)(NULL, neighbour->meta_info);
		
		if (neighbour->status_marker != VERTEX_IN_USE && cmp_result)
			*search_result = neighbour;
			
		
		if (neighbour->run_marker == VERTEX_AVAILABLE)
			__pre_order_traversal_non_recurring_search(neighbour,
						vertex_meta_cmp,
						search_result);
	}
}

Vertex* graph_find_vertex(Graph* graph,
						int start_index,
						int(*vertex_meta_cmp)(void* meta1, void* meta2)){
	
	Vertex* search_result = NULL;
	__pre_order_traversal_search(
						vertex_array_list_get(graph->vertexses, start_index),
						vertex_meta_cmp,
						&search_result);
	
	__graph_drop_runing(graph);
	
	return search_result;
}

Vertex* graph_find_non_recurring_vertex(Graph* graph,
						int start_index,
						int(*vertex_meta_cmp)(void* meta1, void* meta2)){
	Vertex* search_result = NULL;
	__pre_order_traversal_non_recurring_search(
						vertex_array_list_get(graph->vertexses, start_index),
						vertex_meta_cmp,
						&search_result);
	
	if (search_result != NULL)
		vertex_set_status(search_result, VERTEX_IN_USE);
	
	__graph_drop_runing(graph);
	
	return search_result;
}

int __pre_order_traversal_specific_path_search(Vertex* vertex,
					VertexArrayList* ans_list,
					unsigned min_path_len,
					int (*check_specification_fun)(VertexArrayList* ans)){
	
	Vertex* neighbour;
	
	vertex->run_marker = VERTEX_PASSED;
	vertex_set_status(vertex, VERTEX_IN_USE);
	
	vertex_array_list_append(ans_list, vertex);
	
	if (vertex_array_list_len(ans_list) >= min_path_len){
		if ((*check_specification_fun)(ans_list))
			return 1;
		else{
			vertex->run_marker = VERTEX_AVAILABLE;
			vertex_set_status(vertex, VERTEX_FREE);
			vertex_array_list_pop(ans_list, -1);
			return 0;
		}
	}
	
	int len = vertex_array_list_len(vertex->neighbours);
	for (int i = 0; i < len; i++){
		neighbour = vertex_array_list_get(vertex->neighbours, i);

		if (neighbour->run_marker == VERTEX_AVAILABLE &&
			neighbour->status_marker == VERTEX_FREE)
			if (__pre_order_traversal_specific_path_search(neighbour,
									ans_list,
									min_path_len,
									check_specification_fun))
				return 1;
	}
	
	vertex->run_marker = VERTEX_AVAILABLE;
	vertex_set_status(vertex, VERTEX_FREE);
	vertex_array_list_pop(ans_list, -1);
	return 0;
}

VertexArrayList* graph_find_specific_path(Graph* graph,
						unsigned start_index,
						unsigned min_path_len,
						int (*check_specification_fun)(VertexArrayList* ans)){
	Vertex* start_vertex = vertex_array_list_get(graph->vertexses, start_index);
	VertexArrayList* list = vertex_array_list_new(0);
	
	if (__pre_order_traversal_specific_path_search(start_vertex,
									list,
									min_path_len,
									check_specification_fun))
		return list;

	vertex_array_list_del(list);
	return NULL;
}
