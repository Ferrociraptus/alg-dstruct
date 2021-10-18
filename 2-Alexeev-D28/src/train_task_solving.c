#include "train_task_solving.h"

#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "vertex_array_list.h"

#define JOIN_TIME_FROM_META(meta) (*((int*)(meta)))

static int* __joining_time_meta(int time){
	int* meta = (int*)malloc(sizeof(int));
	*meta = time;
	return meta;
}

static unsigned max_time;
static int need_to_send;

static int __is_answer_correct(VertexArrayList* list){
	unsigned summ_time = 0;
	unsigned list_len = vertex_array_list_len(list);
	for (unsigned i = 0; i < list_len; i++){
		Vertex* vertex = vertex_array_list_get(list, i);
		if (vertex != NULL){
			void* meta = vertex_get_meta_info(vertex);
			if (meta != NULL)
				summ_time += JOIN_TIME_FROM_META(meta);
		}
	}
	
	if (summ_time < max_time)
		return 1;
	
	return 0;
}

Graph* parse_train_spec_to_graph(FILE* input_stream){
	// scaning first parameters
	int carriages_amount, summ_send_time, K;
	fscanf(input_stream, "%d %d %d", &carriages_amount, &summ_send_time, &K);
	
	max_time = summ_send_time;
	need_to_send = carriages_amount - K;
	
	Graph* graph = graph_new(carriages_amount+1);
	
	// read vertex weights
	for (unsigned i = 0 ; i < carriages_amount; i ++){
		int joining_time;
		fscanf(input_stream, "%d", &joining_time);
		graph_add_vertex(graph, i+1, __joining_time_meta(joining_time));
	}
	
	// read one derectional edge
	int n;
	fscanf(input_stream, "%d", &n);
	for (unsigned i = 0; i < n; i++){
		int index1, index2;
		fscanf(input_stream, "%d %d", &index1, &index2);
		graph_create_one_derectional_hard_link_edge(graph, index1, index2);
	}
	
	// creating full graph
	for (unsigned i = 1; i <= carriages_amount; i++)
		for (unsigned k = 1; k <= carriages_amount; k++)
			if (i != k && graph_is_vertex_joined(graph, i, k) != GRAPH_TRUE)
				graph_create_biderectional_edge(graph, i, k);
			

	return graph;
}

int organize_train_carriges(const char* train_spec_file,
							const char* soling_file){
	FILE* input = fopen(train_spec_file, "r");
	if (input == NULL)
		return 0;
	
	FILE* output = fopen(soling_file, "w");
	if (output == NULL)
		return 0;
	
	Graph* graph = parse_train_spec_to_graph(input);
	
	// find solving
	Vertex* train_head = vertex_array_list_get(*((VertexArrayList**)graph), 0);
	vertex_set_status(train_head, VERTEX_IN_USE);
	vertex_array_list_print_indexses(graph_find_specific_path(graph, 1, need_to_send, __is_answer_correct), output, " "); 
	
	graph_del(graph);
	fclose(output);
	fclose(input);
	return 1;
}

