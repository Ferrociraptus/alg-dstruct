#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include "memallocator.h"

#define MAX_ALLOCATING_SIZE_IN_TEST 1000
#define MAX_RUN_ITERATIONS 100000
#define TESTS_AMOUNT 100
#define ITERATIONS_IN_TEST (MAX_RUN_ITERATIONS / TESTS_AMOUNT)

#define MEM_BLOCK_SIZE (MAX_ALLOCATING_SIZE_IN_TEST*MAX_RUN_ITERATIONS)

static void test(unsigned int iterations_amount, void* (*alloc_fun)(unsigned long), void(*free_fun)(void*)){
	void* arr[MAX_RUN_ITERATIONS];
	
	for (int i = 0; i < iterations_amount; i++)
			arr[i] = alloc_fun(rand() % MAX_ALLOCATING_SIZE_IN_TEST);
		
	for (int i = 0; i < iterations_amount; i++)
		free_fun(arr[i]);
}

int main(){
	setlocale(LC_ALL, "ru_RU.utf8");
	void* memory_block = malloc(MEM_BLOCK_SIZE);
	meminit(memory_block, MEM_BLOCK_SIZE);
	
	FILE* result_file = fopen("compare.csv", "w");
	if (result_file == NULL)
		return -1;
	
	fprintf(result_file, "Test number\tIterations\tStd result\tLab result\n");
	
	unsigned int loop = 0;
	for (unsigned iters = ITERATIONS_IN_TEST; iters <= MAX_RUN_ITERATIONS; iters+= ITERATIONS_IN_TEST, loop++){
		unsigned long start = clock();
			test(iters/2, malloc, free);
			test(iters/2, malloc, free);
		double std_mem_result = ((double)(clock() - start))/CLOCKS_PER_SEC;
		
		start = clock();
			test(iters/2, memalloc, memfree);
			test(iters/2, memalloc, memfree);
		double lab_mem_result = ((double)(clock() - start))/CLOCKS_PER_SEC;
		fprintf(result_file, "%d\t%u\t%lf\t%lf\n", loop, iters, std_mem_result, lab_mem_result);
		
	}
	memdone();
	fclose(result_file);
	free(memory_block);
	
	return 0;
}
