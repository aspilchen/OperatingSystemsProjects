#ifndef __MEM_BLOCK_H__
#define __MEM_BLOCK_H__

#include <stdlib.h>

enum allocation_flag {
	ALLOCATED,
	UNALLOCATED
};

struct mem_block_t {
	int size;
	enum allocation_flag is_allocated	;
	void* start;
	struct mem_block_t* next;
};

void mem_block_init(struct mem_block_t* blk);
int size_of(struct mem_block_t* blk);
void mem_block_add(struct mem_block_t** head, struct mem_block_t* blk);
void mem_block_remove(struct mem_block_t** head, struct mem_block_t* target);
struct mem_block_t* find_block(struct mem_block_t* head, void* target);
void destroy_block_list(struct mem_block_t* head);

#endif
