#include "gtest/gtest.h"

extern "C" {
#include <stdlib.h>
#include "graph.h"
}

#define INPUT_STREAM_FILE "input.txt"
#define OUTPUT_STREAM_FILE "output.txt"


TEST(pre_order_graph_traversal, example_test1){
	FILE* input = fopen(INPUT_STREAM_FILE, "w");
	fprintf(input, "5\n"	\
					"0 2\n"	\
					"1\n"	\
					"2 4\n"	\
					"3 4\n"	\
					"4\n");
	fclose(input);
	
	input = fopen(INPUT_STREAM_FILE, "r");
	FILE* output = fopen(OUTPUT_STREAM_FILE, "w");
	
	Graph* graph = graph_parse_graph(input);
	VertexArrayList* ans = graph_pre_order_traversal(graph, 0);
	for (int i = 0; i < vertex_array_list_len(ans) - 1; i++)
		fprintf(output, "%d ", vertex_get_index(vertex_array_list_get(ans, i)));
	fprintf(output, "%d\n", vertex_get_index(vertex_array_list_get(ans, vertex_array_list_len(ans) - 1)));
	fclose(output);
	
	output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[50] = {'\0'};
	fgets(line, 50, output);
	
	EXPECT_EQ(strcmp(line, "0 2 4 3\n"), 0);
	
	remove(INPUT_STREAM_FILE);
	remove(OUTPUT_STREAM_FILE);
}

TEST(pre_order_graph_traversal, example_test2){
	FILE* input = fopen(INPUT_STREAM_FILE, "w");

	fprintf(input, "10\n"			\
					"0 5 7 8 9\n"	\
					"1 3 5 6 8 9\n"	\
					"2 4 7\n"		\
					"3 6 8 9\n"		\
					"4 7 8\n"		\
					"5 7\n" 		\
					"6 7\n" 		\
					"7 8\n" 		\
					"8 9\n"			\
					"9\n");
	fclose(input);
	
	input = fopen(INPUT_STREAM_FILE, "r");
	FILE* output = fopen(OUTPUT_STREAM_FILE, "w");
	
	Graph* graph = graph_parse_graph(input);
	VertexArrayList* ans = graph_pre_order_traversal(graph, 0);
	for (int i = 0; i < vertex_array_list_len(ans) - 1; i++)
		fprintf(output, "%d ", vertex_get_index(vertex_array_list_get(ans, i)));
	fprintf(output, "%d\n", vertex_get_index(vertex_array_list_get(ans, vertex_array_list_len(ans) - 1)));
	fclose(output);
	
	output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[50] = {'\0'};
	fgets(line, 50, output);
	
	EXPECT_EQ(strcmp(line, "0 5 1 3 6 7 2 4 8 9\n"), 0);
	
	remove(INPUT_STREAM_FILE);
	remove(OUTPUT_STREAM_FILE);
}

TEST(pre_order_graph_traversal, binary_tree_test1){
	FILE* input = fopen(INPUT_STREAM_FILE, "w");

	fprintf(input, "8\n"		\
					"0 1 2\n"	\
					"1 3 4\n"	\
					"3\n"		\
					"4 5\n"		\
					"5\n"		\
					"2 6 8\n"	\
					"6\n" 		\
					"8\n");
	fclose(input);
	
	input = fopen(INPUT_STREAM_FILE, "r");
	FILE* output = fopen(OUTPUT_STREAM_FILE, "w");
	
	Graph* graph = graph_parse_graph(input);
	VertexArrayList* ans = graph_pre_order_traversal(graph, 0);
	for (int i = 0; i < vertex_array_list_len(ans) - 1; i++)
		fprintf(output, "%d ", vertex_get_index(vertex_array_list_get(ans, i)));
	fprintf(output, "%d\n", vertex_get_index(vertex_array_list_get(ans, vertex_array_list_len(ans) - 1)));
	fclose(output);
	
	output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[50] = {'\0'};
	fgets(line, 50, output);
	
	EXPECT_EQ(strcmp(line, "0 1 3 4 5 2 6 8\n"), 0);
	
	remove(INPUT_STREAM_FILE);
	remove(OUTPUT_STREAM_FILE);
}

TEST(pre_order_graph_traversal, binary_tree_test){
	FILE* input = fopen(INPUT_STREAM_FILE, "w");

	fprintf(input, "8\n"		\
					"0 1 2\n"	\
					"1 3 4\n"	\
					"3\n"		\
					"4 5\n"		\
					"5\n"		\
					"2 6 8\n"	\
					"6\n" 		\
					"8\n");
	fclose(input);
	
	input = fopen(INPUT_STREAM_FILE, "r");
	FILE* output = fopen(OUTPUT_STREAM_FILE, "w");
	
	Graph* graph = graph_parse_graph(input);
	VertexArrayList* ans = graph_pre_order_traversal(graph, 0);
	for (int i = 0; i < vertex_array_list_len(ans) - 1; i++)
		fprintf(output, "%d ", vertex_get_index(vertex_array_list_get(ans, i)));
	fprintf(output, "%d\n", vertex_get_index(vertex_array_list_get(ans, vertex_array_list_len(ans) - 1)));
	fclose(output);
	
	output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[50] = {'\0'};
	fgets(line, 50, output);
	
	EXPECT_EQ(strcmp(line, "0 1 3 4 5 2 6 8\n"), 0);
	
	remove(INPUT_STREAM_FILE);
	remove(OUTPUT_STREAM_FILE);
}

TEST(pre_order_graph_traversal, looped_graph_test){
	FILE* input = fopen(INPUT_STREAM_FILE, "w");

	fprintf(input, "8\n"			\
					"0 1 3\n"		\
					"1 2\n"			\
					"2 6\n"			\
					"3 6 8\n"		\
					"4 3 7\n"		\
					"6 2 0 3\n"		\
					"7 3 2\n"		\
					"8\n");
	fclose(input);
	
	input = fopen(INPUT_STREAM_FILE, "r");
	FILE* output = fopen(OUTPUT_STREAM_FILE, "w");
	
	Graph* graph = graph_parse_graph(input);
	VertexArrayList* ans = graph_pre_order_traversal(graph, 0);
	for (int i = 0; i < vertex_array_list_len(ans) - 1; i++)
		fprintf(output, "%d ", vertex_get_index(vertex_array_list_get(ans, i)));
	fprintf(output, "%d\n", vertex_get_index(vertex_array_list_get(ans, vertex_array_list_len(ans) - 1)));
	fclose(output);
	
	output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[50] = {'\0'};
	fgets(line, 50, output);
	
	EXPECT_EQ(strcmp(line, "0 1 2 6 3 4 7 8\n"), 0);
	
	remove(INPUT_STREAM_FILE);
	remove(OUTPUT_STREAM_FILE);
}

TEST(pre_order_graph_traversal, disconected_graph_test){
	FILE* input = fopen(INPUT_STREAM_FILE, "w");

	fprintf(input, "8\n"			\
					"0 1 7\n"		\
					"1 3\n"			\
					"3 6 7\n"		\
					"6 0\n"			\
					"5 9 8\n"		\
					"7 3 0\n"		\
					"8\n"			\
					"9 5\n");
	fclose(input);
	
	input = fopen(INPUT_STREAM_FILE, "r");
	FILE* output = fopen(OUTPUT_STREAM_FILE, "w");
	
	Graph* graph = graph_parse_graph(input);
	VertexArrayList* ans = graph_pre_order_traversal(graph, 0);
	for (int i = 0; i < vertex_array_list_len(ans) - 1; i++)
		fprintf(output, "%d ", vertex_get_index(vertex_array_list_get(ans, i)));
	fprintf(output, "%d\n", vertex_get_index(vertex_array_list_get(ans, vertex_array_list_len(ans) - 1)));
	fclose(output);
	
	output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[50] = {'\0'};
	fgets(line, 50, output);
	
	EXPECT_EQ(strcmp(line, "0 1 3 6 7\n"), 0);
	
	remove(INPUT_STREAM_FILE);
	remove(OUTPUT_STREAM_FILE);
}

TEST(pre_order_graph_traversal, disconected_graph_determined_start_test){
	FILE* input = fopen(INPUT_STREAM_FILE, "w");

	fprintf(input, "4\n"			\
					"0\n"			\
					"1 2 6\n"		\
					"2\n"			\
					"3 1\n"			\
					"6 2\n");
	fclose(input);
	
	input = fopen(INPUT_STREAM_FILE, "r");
	FILE* output = fopen(OUTPUT_STREAM_FILE, "w");
	
	Graph* graph = graph_parse_graph(input);
	VertexArrayList* ans = graph_pre_order_traversal(graph, 0);
	for (int i = 0; i < vertex_array_list_len(ans) - 1; i++)
		fprintf(output, "%d ", vertex_get_index(vertex_array_list_get(ans, i)));
	fprintf(output, "%d\n", vertex_get_index(vertex_array_list_get(ans, vertex_array_list_len(ans) - 1)));
	fclose(output);
	
	output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[50] = {'\0'};
	fgets(line, 50, output);
	
	EXPECT_EQ(strcmp(line, "0\n"), 0);
	
	remove(INPUT_STREAM_FILE);
	remove(OUTPUT_STREAM_FILE);
}
