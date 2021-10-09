#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

int main(){
	Graph* graph = graph_parse_graph_adjacency_list(stdin);
	VertexArrayList* ans = graph_pre_order_traversal(graph, 0);
	for (int i = 0; i < vertex_array_list_len(ans) - 1; i++)
		printf("%d ", vertex_get_index(vertex_array_list_get(ans, i)));
	printf("%d\n", vertex_get_index(vertex_array_list_get(ans, vertex_array_list_len(ans) - 1)));
	
	graph_del(graph);
	vertex_array_list_del(ans);
	return 0;
}
