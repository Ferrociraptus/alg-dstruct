#include "gtest/gtest.h"

extern "C" {
#include <stdlib.h>
#include <time.h>
#include "train_task_solving.h"
}

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
#define CHECK_LINE_SIZE 100

typedef struct __vertex{
	unsigned int index;
	
	//for running into graph
	RuntimeVertexMarker run_marker;
	
	//for iteration finding values into graph
	StatusVertexMarker status_marker;
	
	VertexArrayList* neighbours;
	VertexArrayList* hard_link_neighbours;
	unsigned block_counter;
	void* meta_info;
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

TEST(train_organize_functional_test_task_sample, example_test){
	FILE* input = fopen(INPUT_STREAM_FILE, "w");
	fprintf(input, "10 40 3\n"	\
					"5 3 3 3 6 7 7 9 10 10\n"	\
					"4\n"	\
					"3 2\n"	\
					"4 2\n"	\
					"10 7\n"\
					"7 9\n");
	fclose(input);
	
	organize_train_carriges(INPUT_STREAM_FILE, OUTPUT_STREAM_FILE);
	
	FILE* output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[CHECK_LINE_SIZE] = {'\0'};
	fgets(line, CHECK_LINE_SIZE, output);
	
	EXPECT_EQ(strcmp(line, "1 3 4 2 5 6 7\n"), 0);

	remove(INPUT_STREAM_FILE);
	remove(OUTPUT_STREAM_FILE);
}


TEST(train_organize_functional_test, user_example_task_test){
	FILE* input = fopen(INPUT_STREAM_FILE, "w");
	fprintf(input, "5 10 2\n"	\
					"5 3 6 2 1\n"	\
					"2\n"	\
					"2 3\n"	\
					"3 5\n");
	fclose(input);
	
	organize_train_carriges(INPUT_STREAM_FILE, OUTPUT_STREAM_FILE);
	
	FILE* output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[CHECK_LINE_SIZE] = {'\0'};
	fgets(line, CHECK_LINE_SIZE, output);
	
	EXPECT_EQ(strcmp(line, "1 2 5\n"), 0);

	remove(INPUT_STREAM_FILE);
	remove(OUTPUT_STREAM_FILE);
}

TEST(train_organize_functional_test_one_carriage_without_link, enough_value_test){
	FILE* input = fopen(INPUT_STREAM_FILE, "w");
	fprintf(input, "1 10 0\n"	\
					"5\n"	\
					"0\n");
	fclose(input);
	
	organize_train_carriges(INPUT_STREAM_FILE, OUTPUT_STREAM_FILE);
	
	FILE* output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[CHECK_LINE_SIZE] = {'\0'};
	fgets(line, CHECK_LINE_SIZE, output);
	
	EXPECT_EQ(strcmp(line, "1\n"), 0);

	remove(INPUT_STREAM_FILE);
	remove(OUTPUT_STREAM_FILE);
}

TEST(train_organize_functional_test_one_carriage_without_link, too_big_value_test){
	FILE* input = fopen(INPUT_STREAM_FILE, "w");
	fprintf(input, "1 10 0\n"	\
					"10\n"	\
					"0\n");
	fclose(input);
	
	organize_train_carriges(INPUT_STREAM_FILE, OUTPUT_STREAM_FILE);
	
	FILE* output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[CHECK_LINE_SIZE] = {'\0'};
	fgets(line, CHECK_LINE_SIZE, output);
	
	EXPECT_EQ(strcmp(line, "0\n"), 0);

	remove(INPUT_STREAM_FILE);
	remove(OUTPUT_STREAM_FILE);
}

TEST(train_organize_functional_test_two_carriages_without_link, enough_value_test){
	FILE* input = fopen(INPUT_STREAM_FILE, "w");
	fprintf(input, "2 10 0\n"	\
					"5 4\n"	\
					"0\n");
	fclose(input);
	
	organize_train_carriges(INPUT_STREAM_FILE, OUTPUT_STREAM_FILE);
	
	FILE* output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[CHECK_LINE_SIZE] = {'\0'};
	fgets(line, CHECK_LINE_SIZE, output);
	
	EXPECT_EQ(strcmp(line, "1 2\n"), 0);

	remove(INPUT_STREAM_FILE);
	remove(OUTPUT_STREAM_FILE);
}

TEST(train_organize_functional_test_two_carriages_without_link, too_big_value_test){
	FILE* input = fopen(INPUT_STREAM_FILE, "w");
	fprintf(input, "2 10 0\n"	\
					"5 5\n"	\
					"0\n");
	fclose(input);
	
	organize_train_carriges(INPUT_STREAM_FILE, OUTPUT_STREAM_FILE);
	
	FILE* output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[CHECK_LINE_SIZE] = {'\0'};
	fgets(line, CHECK_LINE_SIZE, output);
	
	EXPECT_EQ(strcmp(line, "0\n"), 0);

	remove(INPUT_STREAM_FILE);
	remove(OUTPUT_STREAM_FILE);
}

TEST(train_organize_functional_test_two_carriages_with_link, enough_value_test){
	FILE* input = fopen(INPUT_STREAM_FILE, "w");
	fprintf(input, "2 10 0\n"	\
					"5 4\n"	\
					"1\n"	\
					"1 2\n");
	fclose(input);
	
	organize_train_carriges(INPUT_STREAM_FILE, OUTPUT_STREAM_FILE);
	
	FILE* output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[CHECK_LINE_SIZE] = {'\0'};
	fgets(line, CHECK_LINE_SIZE, output);
	
	EXPECT_EQ(strcmp(line, "1 2\n"), 0);

	remove(INPUT_STREAM_FILE);
	remove(OUTPUT_STREAM_FILE);
}

TEST(train_organize_functional_test_two_carriages_with_link, too_big_value_test){
	FILE* input = fopen(INPUT_STREAM_FILE, "w");
	fprintf(input, "2 10 0\n"	\
					"5 5\n"		\
					"1\n"		\
					"1 2\n");
	fclose(input);
	
	organize_train_carriges(INPUT_STREAM_FILE, OUTPUT_STREAM_FILE);
	
	FILE* output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[CHECK_LINE_SIZE] = {'\0'};
	fgets(line, CHECK_LINE_SIZE, output);
	
	EXPECT_EQ(strcmp(line, "0\n"), 0);

	remove(INPUT_STREAM_FILE);
	remove(OUTPUT_STREAM_FILE);
}

TEST(train_organize_functional_test_two_carriages_with_link, inkorrect_link_test){
	FILE* input = fopen(INPUT_STREAM_FILE, "w");
	fprintf(input, "2 10 0\n"	\
					"5 3\n"		\
					"1\n"		\
					"2 1\n");
	fclose(input);
	
	organize_train_carriges(INPUT_STREAM_FILE, OUTPUT_STREAM_FILE);
	
	FILE* output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[CHECK_LINE_SIZE] = {'\0'};
	fgets(line, CHECK_LINE_SIZE, output);
	
	EXPECT_EQ(strcmp(line, "0\n"), 0);

	remove(INPUT_STREAM_FILE);
	remove(OUTPUT_STREAM_FILE);
}

TEST(train_organize_functional_test_two_carriages_big_k, k_equal_to_w){
	FILE* input = fopen(INPUT_STREAM_FILE, "w");
	fprintf(input, "2 10 2\n"	\
					"2 3\n"	\
					"0\n");
	fclose(input);
	
	organize_train_carriges(INPUT_STREAM_FILE, OUTPUT_STREAM_FILE);
	
	FILE* output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[CHECK_LINE_SIZE] = {'\0'};
	fgets(line, CHECK_LINE_SIZE, output);
	
	EXPECT_EQ(strcmp(line, "1\n"), 0);

	remove(INPUT_STREAM_FILE);
	remove(OUTPUT_STREAM_FILE);
}

TEST(train_organize_functional_test_two_carriages_big_k, too_big_value_sum_test){
	FILE* input = fopen(INPUT_STREAM_FILE, "w");
	fprintf(input, "2 10 2\n"	\
					"5 5\n"	\
					"0\n");
	fclose(input);
	
	organize_train_carriges(INPUT_STREAM_FILE, OUTPUT_STREAM_FILE);
	
	FILE* output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[CHECK_LINE_SIZE] = {'\0'};
	fgets(line, CHECK_LINE_SIZE, output);
	
	EXPECT_EQ(strcmp(line, "1\n"), 0);

	remove(INPUT_STREAM_FILE);
	remove(OUTPUT_STREAM_FILE);
}

TEST(train_organize_functional_test_two_carriages_big_k, too_big_value_test){
	FILE* input = fopen(INPUT_STREAM_FILE, "w");
	fprintf(input, "2 10 2\n"	\
					"10 5\n"	\
					"0\n");
	fclose(input);
	
	organize_train_carriges(INPUT_STREAM_FILE, OUTPUT_STREAM_FILE);
	
	FILE* output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[CHECK_LINE_SIZE] = {'\0'};
	fgets(line, CHECK_LINE_SIZE, output);
	
	EXPECT_EQ(strcmp(line, "0\n"), 0);

	remove(INPUT_STREAM_FILE);
	remove(OUTPUT_STREAM_FILE);
}

TEST(train_organize_functional_test_three_carriages_without_link, too_big_value_test){
	FILE* input = fopen(INPUT_STREAM_FILE, "w");
	fprintf(input, "3 3 0\n"	\
					"1 2 3\n"	\
					"0\n");
	fclose(input);
	
	organize_train_carriges(INPUT_STREAM_FILE, OUTPUT_STREAM_FILE);
	
	FILE* output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[CHECK_LINE_SIZE] = {'\0'};
	fgets(line, CHECK_LINE_SIZE, output);
	
	EXPECT_EQ(strcmp(line, "0\n"), 0);

	remove(INPUT_STREAM_FILE);
	remove(OUTPUT_STREAM_FILE);
}

TEST(train_organize_functional_test_three_carriages_without_link, too_small_k){
	FILE* input = fopen(INPUT_STREAM_FILE, "w");
	fprintf(input, "3 3 1\n"	\
					"1 2 3\n"	\
					"0\n");
	fclose(input);
	
	organize_train_carriges(INPUT_STREAM_FILE, OUTPUT_STREAM_FILE);
	
	FILE* output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[CHECK_LINE_SIZE] = {'\0'};
	fgets(line, CHECK_LINE_SIZE, output);
	
	EXPECT_EQ(strcmp(line, "0\n"), 0);

	remove(INPUT_STREAM_FILE);
	remove(OUTPUT_STREAM_FILE);
}

TEST(train_organize_functional_test_three_carriages_without_link, too_big_sum_and_k_smaller_w){
	FILE* input = fopen(INPUT_STREAM_FILE, "w");
	fprintf(input, "3 3 2\n"	\
					"1 2 3\n"	\
					"0\n");
	fclose(input);
	
	organize_train_carriges(INPUT_STREAM_FILE, OUTPUT_STREAM_FILE);
	
	FILE* output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[CHECK_LINE_SIZE] = {'\0'};
	fgets(line, CHECK_LINE_SIZE, output);
	
	EXPECT_EQ(strcmp(line, "1\n"), 0);

	remove(INPUT_STREAM_FILE);
	remove(OUTPUT_STREAM_FILE);
}


TEST(train_organize_functional_test_three_carriages_without_link, too_big_sum_and_k_equal_to_w){
	FILE* input = fopen(INPUT_STREAM_FILE, "w");
	fprintf(input, "3 3 3\n"	\
					"1 2 3\n"	\
					"0\n");
	fclose(input);
	
	organize_train_carriges(INPUT_STREAM_FILE, OUTPUT_STREAM_FILE);
	
	FILE* output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[CHECK_LINE_SIZE] = {'\0'};
	fgets(line, CHECK_LINE_SIZE, output);
	
	EXPECT_EQ(strcmp(line, "1\n"), 0);

	remove(INPUT_STREAM_FILE);
	remove(OUTPUT_STREAM_FILE);
}

TEST(train_organize_functional_test_three_carriages_without_link, train_sum_equal_to_d){
	FILE* input = fopen(INPUT_STREAM_FILE, "w");
	fprintf(input, "3 6 0\n"	\
					"1 2 3\n"	\
					"0\n");
	fclose(input);
	
	organize_train_carriges(INPUT_STREAM_FILE, OUTPUT_STREAM_FILE);
	
	FILE* output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[CHECK_LINE_SIZE] = {'\0'};
	fgets(line, CHECK_LINE_SIZE, output);
	
	EXPECT_EQ(strcmp(line, "0\n"), 0);

	remove(INPUT_STREAM_FILE);
	remove(OUTPUT_STREAM_FILE);
}

TEST(train_organize_functional_test_three_carriages_with_link, one_directional_link){
	FILE* input = fopen(INPUT_STREAM_FILE, "w");
	fprintf(input, "3 7 0\n"	\
					"1 2 3\n"	\
					"1\n"		\
					"2 3");
	fclose(input);
	
	organize_train_carriges(INPUT_STREAM_FILE, OUTPUT_STREAM_FILE);
	
	FILE* output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[CHECK_LINE_SIZE] = {'\0'};
	fgets(line, CHECK_LINE_SIZE, output);
	
	EXPECT_EQ(strcmp(line, "1 2 3\n"), 0);

	remove(INPUT_STREAM_FILE);
	remove(OUTPUT_STREAM_FILE);
}

TEST(train_organize_functional_test_three_carriages_with_link, reverce_directional_link){
	FILE* input = fopen(INPUT_STREAM_FILE, "w");
	fprintf(input, "3 7 0\n"	\
					"1 2 3\n"	\
					"1\n"		\
					"3 2");
	fclose(input);
	
	organize_train_carriges(INPUT_STREAM_FILE, OUTPUT_STREAM_FILE);
	
	FILE* output = fopen(OUTPUT_STREAM_FILE, "r");
	char line[CHECK_LINE_SIZE] = {'\0'};
	fgets(line, CHECK_LINE_SIZE, output);
	
	EXPECT_EQ(strcmp(line, "1 3 2\n"), 0);

	remove(INPUT_STREAM_FILE);
	remove(OUTPUT_STREAM_FILE);
}





/*
[----------] 1 test from train_organize_stress_test
[ RUN      ] train_organize_stress_test.runing_time_stress_test
[       OK ] train_organize_stress_test.runing_time_stress_test (1998120 ms)
[----------] 1 test from train_organize_stress_test (1998165 ms total)

time:
real    33m18.852s
user    33m5.069s
sys     0m3.627s

valgrind:
==77843== 
==77843== HEAP SUMMARY:
==77843==     in use at exit: 231,344 bytes in 38 blocks
==77843==   total heap usage: 221,455 allocs, 221,417 frees, 16,982,885,907 bytes allocated
==77843== 
==77843== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
*/

TEST(train_organize_stress_test, runing_time_stress_test){
	srand(time(NULL));
	
	unsigned long max_time_range_for_node = 100;
	
	unsigned long sum_memory_size = 200 * 1024; // 200Mib
	
//	init graph and answer list
	unsigned int init_memory = sizeof(Graph) + sizeof(Graph*) + sizeof(VertexArrayList) + sizeof(VertexArrayList*);
	
// 	adding vertex into graph vertex list
	unsigned int node_creation = sizeof(Vertex) + sizeof(VertexArrayList);
	
// 	links into each node in neighbours vertex list
	unsigned int one_link_memory = 2 * sizeof(Vertex*);
	
// 	1/2 of all memory to links
	unsigned int links_amount;
	
	unsigned int nodes_amount = links_amount = (sum_memory_size - init_memory)/(node_creation + one_link_memory);
	unsigned int links_in_one_node = links_amount / nodes_amount;
	
	unsigned max_time = nodes_amount * max_time_range_for_node / 2;
	
	FILE* input = fopen(INPUT_STREAM_FILE, "w");
	fprintf(input, "%u %u %u\n", nodes_amount, max_time, nodes_amount/10*8);
	
	for (unsigned node_number = 0; node_number < nodes_amount; node_number++){
		if (node_number < nodes_amount - 1)
			fprintf(input, "%lu ", rand() % max_time_range_for_node);
		else
			fprintf(input, "%lu\n", rand() % max_time_range_for_node);
	}
	
	fprintf(input, "%d\n", nodes_amount / 3);
	
	for (unsigned node_number = 0; node_number < nodes_amount / 3; node_number++){
		unsigned val = rand()%nodes_amount;
		unsigned prev_val = val;
		fprintf(input, "%d", val);
		while ((val = rand()%nodes_amount) == prev_val);
		fprintf(input, " %d", val);
		fprintf(input, "\n");
	}
	fclose(input);
	
	organize_train_carriges(INPUT_STREAM_FILE, OUTPUT_STREAM_FILE);

	remove(INPUT_STREAM_FILE);
	remove(OUTPUT_STREAM_FILE);
}
