#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <locale.h>

#include "BinaryTree.h"
#include "AABinaryTree.h"

#define NUMBERS_VALUE_RANGE 1000
#define TESTS_AMOUNT 100
#define OPERATIONS_MULTYPLAYER 10000

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


int main(){
	setlocale(LC_ALL, "ru_Ru");
	srand(time(NULL));
	
	FILE* csv_table = fopen("Compare_table.csv", "wr");
	fprintf(csv_table, "Test number\tOperations amount\t"					\
					"UsualBinaryTree Insertion\tAABinaryTree Insertion\t"	\
					"UsualBinaryTree Searching\tAABinaryTree Searching\t"	\
					"UsualBinaryTree Removing\tAABinaryTree Removing\n");
	
	BinaryTree tree = NULL;
	AABinaryTree aa_tree = NULL;
	aa_binary_tree_init(&aa_tree);
	
	for (int test = 1; test <= TESTS_AMOUNT; test++){
		clock_t begin, end;
		double usual_binary_tree_time_result, aa_binary_tree_time_result;
		
		unsigned long operations_amount = test * OPERATIONS_MULTYPLAYER;
		
		fprintf(csv_table, "%d\t%lu\t", test, operations_amount);
		
		// Insertion check
		{
			begin = clock();
			for (int i = 0; i < operations_amount; i++)
				binary_tree_insert(&tree, rand() % NUMBERS_VALUE_RANGE);
			end = clock();
			usual_binary_tree_time_result = (double)(end - begin) / CLOCKS_PER_SEC;
			
			begin = clock();
			for (int i = 0; i < operations_amount; i++)
				aa_binary_tree_insert(&aa_tree, rand() % NUMBERS_VALUE_RANGE);
			end = clock();
			aa_binary_tree_time_result = (double)(end - begin) / CLOCKS_PER_SEC;
		
			fprintf(csv_table, "%lf\t%lf\t", usual_binary_tree_time_result, aa_binary_tree_time_result);
		}
		
		// Searching check
		{
			begin = clock();
			for (int i = 0; i < operations_amount; i++)
				binary_tree_search(tree, rand() % NUMBERS_VALUE_RANGE);
			end = clock();
			usual_binary_tree_time_result = (double)(end - begin) / CLOCKS_PER_SEC;
			
			begin = clock();
			for (int i = 0; i < operations_amount; i++)
				aa_binary_tree_search(aa_tree, rand() % NUMBERS_VALUE_RANGE);
			end = clock();
			aa_binary_tree_time_result = (double)(end - begin) / CLOCKS_PER_SEC;
		
			fprintf(csv_table, "%lf\t%lf\t", usual_binary_tree_time_result, aa_binary_tree_time_result);
		}
		
		// Removing check
		{
			begin = clock();
			for (int i = 0; i < operations_amount; i++)
				binary_tree_remove(&tree, rand() % NUMBERS_VALUE_RANGE);
			end = clock();
			usual_binary_tree_time_result = (double)(end - begin) / CLOCKS_PER_SEC;
			
			begin = clock();
			for (int i = 0; i < operations_amount; i++)
				aa_binary_tree_remove(&aa_tree, rand() % NUMBERS_VALUE_RANGE);
			end = clock();
			aa_binary_tree_time_result = (double)(end - begin) / CLOCKS_PER_SEC;
		
			fprintf(csv_table, "%lf\t%lf\n", usual_binary_tree_time_result, aa_binary_tree_time_result);
		}
		binary_tree_clear(&tree);
		aa_binary_tree_clear(&aa_tree);
		printf("Test# %d/%d:\tDone\n", test, TESTS_AMOUNT);
	}
	fclose(csv_table);
	return 0;
}
