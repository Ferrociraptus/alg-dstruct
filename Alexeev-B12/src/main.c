#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include "memallocator.h"

#define MAX_ALLOCATING_SIZE_IN_TEST 1000
#define MAX_RUN_ITERATIONS 100000
#define TESTS_AMOUNT 100
#define ITERATIONS_IN_TEST (MAX_RUN_ITERATIONS / TESTS_AMOUNT)

#define HEADER_SIZE 16
#define MEM_BLOCK_SIZE ((unsigned long)(MAX_ALLOCATING_SIZE_IN_TEST + HEADER_SIZE) * MAX_RUN_ITERATIONS)

static void malloc_test(unsigned int iterations_amount){
	void* arr[MAX_RUN_ITERATIONS] = {NULL};
	int last_i = 0;
	for (int i = 0; i < iterations_amount; i++){
		if (rand()%3 != 0){
			arr[i] = malloc(rand() % MAX_ALLOCATING_SIZE_IN_TEST);
			last_i = i;
		}
		else if (arr[last_i] != NULL){
			free(arr[last_i]);
			arr[last_i] = NULL;
		}
	}
		
	for (int i = 0; i < iterations_amount; i++)
		free(arr[i]);
}

static void memalloc_test(unsigned int iterations_amount){
	void* arr[MAX_RUN_ITERATIONS] = {NULL};
	int last_i = 0;
	for (int i = 0; i < iterations_amount; i++){
		if (rand()%3 != 0){
			arr[i] = memalloc(rand() % MAX_ALLOCATING_SIZE_IN_TEST);
			last_i = i;
		}
		else if (arr[last_i] != NULL){
			memfree(arr[last_i]);
			arr[last_i] = NULL;
		}
	}
	
	for (int i = 0; i < iterations_amount; i++)
		memfree(arr[i]);
}

int main(){
	setlocale(LC_ALL, "ru_RU.utf8");
	
	void* memory_block = malloc(MEM_BLOCK_SIZE);
	meminit(memory_block, MEM_BLOCK_SIZE);
	printf("Memory pool was initialized: %lu bytes\n", MEM_BLOCK_SIZE);
	
	FILE* result_file = fopen("compare.csv", "w");
	if (result_file == NULL)
		return -1;
	
	fprintf(result_file, "Test number\tIterations\tStd result\tLab result\n");
	
	unsigned int loop = 0;
	for (unsigned iters = ITERATIONS_IN_TEST; iters <= MAX_RUN_ITERATIONS; iters+= ITERATIONS_IN_TEST, loop++){
		printf("\tLoop #%u\n", loop);
		unsigned long start = clock();
			malloc_test(iters);
		double std_mem_result = ((double)(clock() - start))/CLOCKS_PER_SEC;
		
		start = clock();
			memalloc_test(iters);
		double lab_mem_result = ((double)(clock() - start))/CLOCKS_PER_SEC;
		fprintf(result_file, "%d\t%u\t%lf\t%lf\n", loop, iters, std_mem_result, lab_mem_result);
		
	}
	memdone();
	fclose(result_file);
	free(memory_block);
	return 0;
}
