#include "sys/resource.h"
#include "gtest/gtest.h"

extern "C" {
#include <stdlib.h>
#include <time.h>
#include "graph.h"
}

#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define GTEST_PRINTF_INFO(str,...) {fprintf(stderr, ANSI_COLOR_YELLOW "[   INFO   ] "); fprintf(stderr, str, __VA_ARGS__); fprintf(stderr, ANSI_COLOR_RESET);}

/*
OS: Fedora 34 (KDE Plasma) x86_64 
Kernel: 5.14.10-200.fc34.x86_64 

memory:
RANGE                                 SIZE  STATE REMOVABLE BLOCK
0x0000000000000000-0x000000007fffffff   2G online       yes  0-15
0x0000000100000000-0x000000027fffffff   6G online       yes 32-79

Memory block size:       128M
Total online memory:       8G
Total offline memory:      0B


CPU: 
Architecture:                    x86_64
CPU op-mode(s):                  32-bit, 64-bit
Byte Order:                      Little Endian
Address sizes:                   39 bits physical, 48 bits virtual
CPU(s):                          8
On-line CPU(s) list:             0-7
Thread(s) per core:              2
Core(s) per socket:              4
Socket(s):                       1
NUMA node(s):                    1
Vendor ID:                       GenuineIntel
CPU family:                      6
Model:                           158
Model name:                      Intel(R) Core(TM) i7-7700HQ CPU @ 2.80GHz
Stepping:                        9
CPU MHz:                         2800.000
BogoMIPS:                        5599.85
Virtualization:                  VT-x
L1d cache:                       128 KiB
L1i cache:                       128 KiB
L2 cache:                        1 MiB
L3 cache:                        6 MiB
NUMA node0 CPU(s):               0-7
*/

#define INPUT_STREAM_FILE "input.txt"
#define OUTPUT_STREAM_FILE "output.txt"

typedef struct __vertex{
	unsigned int index;
	VertexMarker marker;
	VertexArrayList* neighbours;
} Vertex;

struct __vertex_array_list{
	unsigned int size; //size of allocated memory
	unsigned int len;  //amount of elements in list
	Vertex** arr;
};

struct __graph{
	unsigned int max_index;
	VertexArrayList* vertexses;
};

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
	
	Graph* graph = graph_parse_graph_adjacency_list(input);
	VertexArrayList* ans = graph_pre_order_traversal(graph, 0);
	for (int i = 0; i < vertex_array_list_len(ans) - 1; i++)
		fprintf(output, "%d ", vertex_get_index(vertex_array_list_get(ans, i)));
	fprintf(output, "%d\n", vertex_get_index(vertex_array_list_get(ans, vertex_array_list_len(ans) - 1)));
	fclose(output);
	
	output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[50] = {'\0'};
	fgets(line, 50, output);
	
	EXPECT_EQ(strcmp(line, "0 2 4 3\n"), 0);
	
	graph_del(graph);
	vertex_array_list_del(ans);
	
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
	
	Graph* graph = graph_parse_graph_adjacency_list(input);
	VertexArrayList* ans = graph_pre_order_traversal(graph, 0);
	for (int i = 0; i < vertex_array_list_len(ans) - 1; i++)
		fprintf(output, "%d ", vertex_get_index(vertex_array_list_get(ans, i)));
	fprintf(output, "%d\n", vertex_get_index(vertex_array_list_get(ans, vertex_array_list_len(ans) - 1)));
	fclose(output);
	
	output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[50] = {'\0'};
	fgets(line, 50, output);
	
	EXPECT_EQ(strcmp(line, "0 5 1 3 6 7 2 4 8 9\n"), 0);
	
	graph_del(graph);
	vertex_array_list_del(ans);
	
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
	
	Graph* graph = graph_parse_graph_adjacency_list(input);
	VertexArrayList* ans = graph_pre_order_traversal(graph, 0);
	for (int i = 0; i < vertex_array_list_len(ans) - 1; i++)
		fprintf(output, "%d ", vertex_get_index(vertex_array_list_get(ans, i)));
	fprintf(output, "%d\n", vertex_get_index(vertex_array_list_get(ans, vertex_array_list_len(ans) - 1)));
	fclose(output);
	
	output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[50] = {'\0'};
	fgets(line, 50, output);
	
	EXPECT_EQ(strcmp(line, "0 1 3 4 5 2 6 8\n"), 0);
	
	graph_del(graph);
	vertex_array_list_del(ans);
	
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
	
	Graph* graph = graph_parse_graph_adjacency_list(input);
	VertexArrayList* ans = graph_pre_order_traversal(graph, 0);
	for (int i = 0; i < vertex_array_list_len(ans) - 1; i++)
		fprintf(output, "%d ", vertex_get_index(vertex_array_list_get(ans, i)));
	fprintf(output, "%d\n", vertex_get_index(vertex_array_list_get(ans, vertex_array_list_len(ans) - 1)));
	fclose(output);
	
	output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[50] = {'\0'};
	fgets(line, 50, output);
	
	EXPECT_EQ(strcmp(line, "0 1 3 4 5 2 6 8\n"), 0);
	
	graph_del(graph);
	vertex_array_list_del(ans);
	
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
	
	Graph* graph = graph_parse_graph_adjacency_list(input);
	VertexArrayList* ans = graph_pre_order_traversal(graph, 0);
	for (int i = 0; i < vertex_array_list_len(ans) - 1; i++)
		fprintf(output, "%d ", vertex_get_index(vertex_array_list_get(ans, i)));
	fprintf(output, "%d\n", vertex_get_index(vertex_array_list_get(ans, vertex_array_list_len(ans) - 1)));
	fclose(output);
	
	output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[50] = {'\0'};
	fgets(line, 50, output);
	
	EXPECT_EQ(strcmp(line, "0 1 2 6 3 4 7 8\n"), 0);
	
	graph_del(graph);
	vertex_array_list_del(ans);
	
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
	
	Graph* graph = graph_parse_graph_adjacency_list(input);
	VertexArrayList* ans = graph_pre_order_traversal(graph, 0);
	for (int i = 0; i < vertex_array_list_len(ans) - 1; i++)
		fprintf(output, "%d ", vertex_get_index(vertex_array_list_get(ans, i)));
	fprintf(output, "%d\n", vertex_get_index(vertex_array_list_get(ans, vertex_array_list_len(ans) - 1)));
	fclose(output);
	
	output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[50] = {'\0'};
	fgets(line, 50, output);
	
	EXPECT_EQ(strcmp(line, "0 1 3 6 7\n"), 0);
	
	graph_del(graph);
	vertex_array_list_del(ans);
	
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
	
	Graph* graph = graph_parse_graph_adjacency_list(input);
	VertexArrayList* ans = graph_pre_order_traversal(graph, 0);
	for (int i = 0; i < vertex_array_list_len(ans) - 1; i++)
		fprintf(output, "%d ", vertex_get_index(vertex_array_list_get(ans, i)));
	fprintf(output, "%d\n", vertex_get_index(vertex_array_list_get(ans, vertex_array_list_len(ans) - 1)));
	fclose(output);
	
	output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[50] = {'\0'};
	fgets(line, 50, output);
	
	EXPECT_EQ(strcmp(line, "0\n"), 0);
	
	graph_del(graph);
	vertex_array_list_del(ans);
	
	remove(INPUT_STREAM_FILE);
	remove(OUTPUT_STREAM_FILE);
}


/*

[----------] 1 test from pre_order_graph_traversal_stress_execution
[ RUN      ] pre_order_graph_traversal_stress_execution.memory_stress_test
[   INFO   ] Memory stack was increased to: 512Mib
[   INFO   ] Vertex amount: 655358
[       OK ] pre_order_graph_traversal_stress_execution.memory_stress_test (1852573 ms)
[----------] 1 test from pre_order_graph_traversal_stress_execution (1867586 ms total)


valgrind:
==233008== 
==233008== HEAP SUMMARY:
==233008==     in use at exit: 41,964,584 bytes in 31 blocks
==233008==   total heap usage: 2,642,582 allocs, 2,642,551 frees, 430,928,425,445 bytes allocated
==233008== 
==233008== For lists of detected and suppressed errors, rerun with: -s
==233008== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

time:
real    31m9.120s
user    26m47.701s
sys     3m20.295s

*/

TEST(pre_order_graph_traversal_stress_execution, memory_stress_test){
	rlimit rlim;
	
	// increasing stack size
	if (getrlimit(RLIMIT_STACK, &rlim)) exit(1);
	
	rlim.rlim_cur = 512 * 1024 * 1024; // 512 Mib
	
	if (setrlimit(RLIMIT_STACK, &rlim)) exit(2);
	GTEST_PRINTF_INFO("Memory stack was increased to: %luMib\n", rlim.rlim_cur /1024 /1024);
	
	srand(time(NULL));
	
	unsigned long sum_memory_size = 40 * 1024 * 1024; // 40Mib
	
	// init graph and answer list
	unsigned int init_memory = sizeof(Graph) + sizeof(Graph*) + sizeof(VertexArrayList) + sizeof(VertexArrayList*);
	
	// adding vertex into graph vertex list
	unsigned int node_creation = sizeof(Vertex) + sizeof(VertexArrayList);
	
	// links into each node in neighbours vertex list
	unsigned int one_link_memory = 2 * sizeof(Vertex*);
	
	// 1/2 of all memory to links
	unsigned int links_amount = sum_memory_size / 2 * 1 / one_link_memory ;
	
	unsigned int nodes_amount = (sum_memory_size - init_memory - links_amount*one_link_memory)/node_creation;
	unsigned int links_in_one_node = links_amount / nodes_amount;
	
	GTEST_PRINTF_INFO("Vertex amount: %u\n", nodes_amount);
	
	FILE* input = fopen(INPUT_STREAM_FILE, "w");
	fprintf(input, "%u\n", nodes_amount);
	
	for (unsigned node_number = 0; node_number < nodes_amount; node_number++){
		fprintf(input, "%d", node_number);
		for (unsigned links_amount = 0; links_amount <= links_in_one_node; links_amount++)
			fprintf(input, " %d", rand()%nodes_amount);
		fprintf(input, "\n");
	}
	fclose(input);
	
	input = fopen(INPUT_STREAM_FILE, "r");
	FILE* output = fopen(OUTPUT_STREAM_FILE, "w");
	
	Graph* graph = graph_parse_graph_adjacency_list(input);
	VertexArrayList* ans = graph_pre_order_traversal(graph, 0);
	for (int i = 0; i < vertex_array_list_len(ans) - 1; i++)
		fprintf(output, "%d ", vertex_get_index(vertex_array_list_get(ans, i)));
	fprintf(output, "%d\n", vertex_get_index(vertex_array_list_get(ans, vertex_array_list_len(ans) - 1)));
	fclose(output);
	
	graph_del(graph);
	vertex_array_list_del(ans);

	remove(INPUT_STREAM_FILE);
	remove(OUTPUT_STREAM_FILE);
}
