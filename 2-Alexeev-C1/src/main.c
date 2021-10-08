#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

int main(){
// 	FILE* input = fopen("input", "w");
// 
// 	fprintf(input, "8\n"		\
// 					"0 1 2\n"	\
// 					"1 3 4\n"	\
// 					"3\n"		\
// 					"4 5\n"		\
// 					"5\n"		\
// 					"2 6 8\n"	\
// 					"6\n" 		\
// 					"8\n");
// 	fclose(input);
// 	
// 	stdin = freopen("input", "r", stdin);
	
	Graph* graph = graph_parse_graph(stdin);
	VertexArrayList* ans = graph_pre_order_traversal(graph, 0);
	for (int i = 0; i < vertex_array_list_len(ans) - 1; i++)
		printf("%d ", vertex_get_index(vertex_array_list_get(ans, i)));
	printf("%d\n", vertex_get_index(vertex_array_list_get(ans, vertex_array_list_len(ans) - 1)));

// 	remove("input");
	return 0;
}
