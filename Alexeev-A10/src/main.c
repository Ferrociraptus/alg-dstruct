#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ObjectLinkedStack.h"
#include "ObjectArrayStack.h"

#define LONG_OBJECT(obj) ((IntObject*)(obj))

// Simple object for testing list
typedef struct{
	Object obj;
	long val;
} LongObject;

// Simple IntObject constructor
DEFAULT_MEMORY_ALLOCATOR(LongObject)
CONSTRUCTOR(LongObject, long val){
	if (self == NULL)
		return NULL;
	SUPER(Object)(OBJECT(self));
	self->val = val;
	return self;
}

int main(int argc, char** argv){
	FILE* csv_table = fopen("Compare_table.csv", "wr");
	fprintf(csv_table, "Test number\tOperations amount\tLinkedStack\tArrayStack\n");
	
	for (int i = 1; i <= 100; i++){
		clock_t begin, end;
		double linked_stack_time_result, array_stack_time_result;
		
		const unsigned long operations_amount = i * 10;
		
		// Linked stack benchmark
		{
			begin = clock();
			ObjectLinkedStack* stack = new(ObjectLinkedStack);
			
			for (long k = 0; k < operations_amount; k++)
				object_linked_stack_add(stack, OBJECT(new(LongObject, k)));
			
			for (long k = 0; k < operations_amount/2; k++)
				del(object_linked_stack_pop(stack));
			
			del(stack);
			end = clock();
			linked_stack_time_result = (double)(end - begin) / CLOCKS_PER_SEC;
		}
		
		// Array stack benchmark
		{
			
			begin = clock();
			ObjectArrayStack* stack = new(ObjectArrayStack);
			
			for (long k = 0; k < operations_amount; k++)
				object_array_stack_add(stack, OBJECT(new(LongObject, k)));
			
			for (long k = 0; k < operations_amount/2; k++)
				del(object_array_stack_pop(stack));
			
			del(stack);
			
			end = clock();
			array_stack_time_result = (double)(end - begin) / CLOCKS_PER_SEC;
		}
		fprintf(csv_table, "%d\t%lu\t%lf\t%lf\n", i, operations_amount, linked_stack_time_result, array_stack_time_result);
	}
	fclose(csv_table);
	return 0;
}
