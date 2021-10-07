#include "gtest/gtest.h"

extern "C" {
#include <stdlib.h>
#include "memallocator.h"
}



TEST(custom_memory_allocator_testing, first_block_freeing_and_allocating_test) {
	int size = memgetblocksize() * 5 + sizeof(int) * 10;
	void* memory_block = malloc(size);
	meminit(memory_block, size);
	int* p1 = (int*)memalloc(sizeof(int)*2);
	int* p2 = (int*)memalloc(sizeof(int)*2);
	int* p3 = (int*)memalloc(sizeof(int)*2);
	int* p4 = (int*)memalloc(sizeof(int)*2);
	int* p5 = (int*)memalloc(sizeof(int)*2);

	memfree(p2);
	memfree(p1);
	
	p1 = (int*)memalloc(sizeof(int)*3);
	p2 = (int*)memalloc(sizeof(int));
	
	memdone();
	free(memory_block);
}


