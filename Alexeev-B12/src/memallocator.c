#include "memallocator.h"
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#ifndef TRUE
	#define TRUE 1
#endif

#ifndef FALSE
	#define FALSE (!(TRUE))
#endif

#define BYTE_ORDERING(pointer) ((byte*)(pointer))
#define FIRST_CELL_BYTE(header) (BYTE_ORDERING(header) + sizeof(Header))
#define CELL_HEADER(cell_p) ((Header*)(BYTE_ORDERING(cell_p) - sizeof(Header)))

typedef struct __header Header;
typedef uint8_t byte;


static Header* start_head = NULL;
static Header* current_head = NULL;
static uint64_t init_size = 0;


struct __header{
	struct __header* next;
	uint32_t size;
	uint8_t is_reserved: 1;
};

inline static byte* __get_cell_byte(Header* cell_meta, uint32_t byte_index){
	return BYTE_ORDERING(cell_meta) + sizeof(Header) + sizeof(byte) * byte_index;
}

inline static byte* __last_cell_byte(Header* cell_meta){
	if (cell_meta->size > 0)
		return __get_cell_byte(cell_meta, cell_meta->size - 1);
	else
		return BYTE_ORDERING(cell_meta) + sizeof(Header);
}

inline static int8_t __is_mergeble(Header* first, Header* second){
	if (BYTE_ORDERING(__last_cell_byte(first)) + ((first->size > 0) ? sizeof(byte): 0) == BYTE_ORDERING (second))
		return TRUE;
	return FALSE;
}

static uint64_t __mem_pool_size(){
	Header* head = start_head;
	uint64_t size = 0;
	while (head != NULL){
		size += head->size + sizeof(Header);
		head = head->next;
	}
	return size;
}

static Header* __split_cell(Header* cell_meta, uint32_t size){
	if (cell_meta->size <= size)
		return NULL;
	
	// check if cell meta can be placed into tail memory cell of current block
	byte* new_cell_pos = __get_cell_byte(cell_meta, size);
	if ((__last_cell_byte(cell_meta) - new_cell_pos) + 1 <= sizeof(Header))
		return NULL;
	
	Header* new_cell_header = (Header*)new_cell_pos;
	new_cell_header->next = cell_meta->next;
	new_cell_header->is_reserved = FALSE;
	new_cell_header->size = cell_meta->size - size - sizeof(Header);
	
	cell_meta->next = new_cell_header;
	cell_meta->size = size;
	
	return new_cell_header;
}

static int8_t __hard_merge_cells(Header* first, Header* second){
	if (__is_mergeble(first, second)){
		first->next = second->next;
		first->size += second->size + sizeof(Header);
		return TRUE;
	}
	return FALSE;
}

static int8_t __soft_merge_cells(Header* first, Header* second){
	if (first == NULL || second == NULL)
		return FALSE;
	
	if (!first->is_reserved && !second->is_reserved)
		return __hard_merge_cells(first, second);
	return FALSE;
}

static Header* __find_previous_cell(Header* current_cell_meta){
	Header* prev_cell = start_head;
	
	if (current_cell_meta == NULL)
		return NULL;
	
	while (prev_cell->next != NULL){
		if (prev_cell->next == current_cell_meta)
			return prev_cell;
		prev_cell = prev_cell->next;
	}
	return NULL;
}

int meminit(void* pMemory, int size){
	start_head = (Header*)pMemory;
	start_head->size = size - sizeof(Header);
	start_head->next = NULL;
	start_head->is_reserved = FALSE;
	
	current_head = start_head;
	
	init_size = size;
	return 1;
}

void memdone(){
	uint64_t current_size = __mem_pool_size();
	if (current_size != init_size)
		fprintf(stderr, "Memory leak was detected!\n\tExpected: %lu\n\tGot: %lu\n", init_size, current_size);
	start_head = NULL;
	current_head = start_head;
}

void* memalloc(int size){
	Header* heads_cursor = current_head;
	while (heads_cursor != NULL){
		if ( !heads_cursor->is_reserved && heads_cursor->size >= size){
			Header* split_result = __split_cell(heads_cursor, size);
			heads_cursor->is_reserved = TRUE;
			if (current_head->is_reserved && split_result != NULL)
				current_head = split_result;
			return FIRST_CELL_BYTE(heads_cursor);
		}
		heads_cursor = heads_cursor->next;
	}
	fprintf(stderr, "No available memory for %d bytes!\n", size);
	return NULL;
}

void memfree(void* p){
	if (p == NULL)
		return;
	
	Header* current_cell = CELL_HEADER(p);
	
	current_cell->is_reserved = FALSE;
	Header* prev_cell = __find_previous_cell(current_cell);
	
	if (prev_cell != NULL)
		if (__soft_merge_cells(prev_cell, current_cell))
			current_cell = prev_cell;
	
	if (current_cell->next != NULL)
		__soft_merge_cells(current_cell, current_cell->next);
	
	if (current_cell - current_head < 0)
		current_head = current_cell;
}

int memgetminimumsize(){
	return sizeof(Header);
}

int memgetblocksize(){
	return sizeof(Header);
}

