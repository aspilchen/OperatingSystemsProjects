#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kallocator.h"
#include "mem_block.h"

struct KAllocator
{
    enum allocation_algorithm aalgorithm;
    int size;
    void* memory;
		int free_memory;
		struct mem_block_t* free_blocks;
		struct mem_block_t* allocated_blocks;
};

struct KAllocator kallocator;

void initialize_allocator(int _size, enum allocation_algorithm _aalgorithm)
{
    assert(_size > 0);
    kallocator.aalgorithm = _aalgorithm;
    kallocator.size = _size;
    kallocator.memory = malloc((size_t)kallocator.size);
		kallocator.free_memory = _size;
		struct mem_block_t* block = (struct mem_block_t*) malloc(sizeof(struct mem_block_t));
		mem_block_init(block);
		block->size = _size;
		block->start = kallocator.memory;
		kallocator.free_blocks = block;
		kallocator.allocated_blocks = NULL;
}

void destroy_allocator()
{
	destroy_block_list(kallocator.free_blocks);
	kallocator.free_blocks = NULL;
	destroy_block_list(kallocator.allocated_blocks);
	kallocator.allocated_blocks = NULL;
	free(kallocator.memory);
}

struct mem_block_t* find_first_fit(int size)
{
	struct mem_block_t* free_block = NULL;
	if(size < kallocator.free_memory){
		free_block = kallocator.free_blocks;
		while(free_block != NULL && size_of(free_block) < size){
			free_block = free_block->next;
		}
	}
	return free_block;
}

struct mem_block_t* find_best_fit(int size)
{
	struct mem_block_t* free_blk = NULL;
	struct mem_block_t* best_fit = NULL;
	if(size < kallocator.free_memory){
		free_blk = kallocator.free_blocks;
		best_fit = kallocator.free_blocks;
		while(free_blk != NULL){
			if(size <= free_blk->size && free_blk->size < best_fit->size){
				best_fit = free_blk;
			}
			free_blk = free_blk->next;
		}
	}
return best_fit;
}

struct mem_block_t* find_worst_fit(int size)
{
	struct mem_block_t* free_blk = NULL;
	struct mem_block_t* worst_fit = NULL;
	if(size < kallocator.free_memory){
		free_blk = kallocator.free_blocks;
		worst_fit = kallocator.free_blocks;
		while(free_blk != NULL){
			if(size <= free_blk->size && worst_fit->size < free_blk->size ){
				worst_fit = free_blk;
			}
			free_blk = free_blk->next;
		}
	}
return worst_fit;
}

void* kalloc(int _size)
{
	void* ptr = NULL;
	struct mem_block_t* free_blk = NULL;
	struct mem_block_t* new_blk = NULL;
	if(_size > 0){
		switch(kallocator.aalgorithm){
			case FIRST_FIT:
				free_blk = find_first_fit(_size);
				break;
			case BEST_FIT:
				free_blk = find_best_fit(_size);
				break;
			case WORST_FIT:
				free_blk = find_worst_fit(_size);
				break;
		}
    // Allocate memory from kallocator.memory 
    // ptr = address of allocated memory
		if(free_blk != NULL){
			new_blk = (struct mem_block_t*) malloc(sizeof(struct mem_block_t));
			mem_block_init(new_blk);

			new_blk->size = _size;
			new_blk->start = free_blk->start;
			new_blk->next = kallocator.allocated_blocks;

			mem_block_add(&(kallocator.allocated_blocks), new_blk);
			free_blk->start += _size;
			free_blk->size -= _size;
			kallocator.free_memory -= (int) _size;
			ptr = new_blk->start;
		}
	}
	return ptr;
}

void kfree(void* _ptr) 
{
	assert(_ptr != NULL);
	struct mem_block_t* allocated_blk = find_block(kallocator.allocated_blocks, _ptr);
	struct mem_block_t* free_blk = NULL;
	void* next_blk_start;
	if(allocated_blk != NULL){
		next_blk_start = allocated_blk->start + allocated_blk->size;
		free_blk = find_block(kallocator.free_blocks, next_blk_start);

		mem_block_remove(&(kallocator.allocated_blocks), allocated_blk);
		kallocator.free_memory += allocated_blk->size;

		if(free_blk != NULL){
			free_blk->start = allocated_blk->start;
			free_blk->size += allocated_blk->size;
			free(allocated_blk);
		}
		else{
			mem_block_add(&(kallocator.free_blocks), allocated_blk);
		}
	}
}

void shift_blocks(struct mem_block_t* start_blk, int offset)
{
	struct mem_block_t* allocated_blk = start_blk;
	struct mem_block_t* free_blk = NULL;
	void* next_blk_start = start_blk->start + size_of(start_blk);

	while(allocated_blk != NULL || free_blk != NULL){
		if(allocated_blk != NULL){
			next_blk_start = allocated_blk->start + size_of(allocated_blk);
			allocated_blk->start -= offset;
		}
		else if(free_blk != NULL){
			next_blk_start = free_blk->start + size_of(free_blk);
			free_blk->start -= offset;
		}
		allocated_blk = find_block(kallocator.allocated_blocks, next_blk_start);
		free_blk = find_block(kallocator.free_blocks, next_blk_start);
	}
}

void defrag()
{
	struct mem_block_t* allocated_blk = find_block(kallocator.allocated_blocks, kallocator.memory);
	struct mem_block_t* free_blk = find_block(kallocator.free_blocks, kallocator.memory);
	void* next_blk_start = NULL;
	void* new_free_blk_start = NULL;
	int remaining_size;
	int offset = 0;
	int in_fragment = 0;

	while(allocated_blk != NULL || free_blk != NULL){
		if(allocated_blk != NULL){
			if(in_fragment){
				in_fragment = 0;
				remaining_size = (int) ((kallocator.memory + kallocator.size) - allocated_blk->start);
				memcpy(allocated_blk->start - offset, allocated_blk->start, remaining_size);
				shift_blocks(allocated_blk, offset);
			}
			next_blk_start = allocated_blk->start + size_of(allocated_blk);
		}
		else if(free_blk != NULL){
			in_fragment = 1;
			offset = size_of(free_blk);
			next_blk_start = free_blk->start + size_of(free_blk);
		}
		allocated_blk = find_block(kallocator.allocated_blocks, next_blk_start);
		free_blk = find_block(kallocator.free_blocks, next_blk_start);
	}

	new_free_blk_start = kallocator.memory + kallocator.size - kallocator.free_memory;
	destroy_block_list(kallocator.free_blocks);
	kallocator.free_blocks = NULL;
	free_blk = (struct mem_block_t*) malloc(sizeof(struct mem_block_t));
	free_blk->start = new_free_blk_start;
	free_blk->size = kallocator.free_memory;
	mem_block_add(&kallocator.free_blocks, free_blk);
}

			

int compact_allocation(void** _before, void** _after) 
{
	int compact_size;
	struct mem_block_t* allocated_blk = kallocator.allocated_blocks;
	int i = 0;
	while(allocated_blk != NULL){
		_before[i] = allocated_blk->start;
		allocated_blk = allocated_blk->next;
		i++;
	}

	defrag();

	allocated_blk = kallocator.allocated_blocks;
	i = 0;
	while(allocated_blk != NULL){
		_after[i] = allocated_blk->start;
		allocated_blk = allocated_blk->next;
		i++;
	}

	compact_size = i;
	return compact_size;
}

int available_memory() 
{
    // Calculate available memory size
    return kallocator.free_memory;
}

int calculate_allocated_chunks()
{
	int chunks = 0;
	int in_chunk = 0;
	struct mem_block_t* allocated_blk = find_block(kallocator.allocated_blocks, kallocator.memory);
	struct mem_block_t* free_blk = find_block(kallocator.free_blocks, kallocator.memory);
	void* next_blk_start = NULL;

	while(allocated_blk != NULL || free_blk != NULL){
		if(allocated_blk != NULL){
			if(!in_chunk){
				in_chunk = 1;
				chunks++;
			}
			next_blk_start = allocated_blk->start + size_of(allocated_blk);
		}
		else if(free_blk != NULL){
			if(in_chunk){
				in_chunk = 0;
			}
			next_blk_start = free_blk->start + size_of(free_blk);
		}
		allocated_blk = find_block(kallocator.allocated_blocks, next_blk_start);
		free_blk = find_block(kallocator.free_blocks, next_blk_start);
	}
	return chunks;
}

int calculate_free_chunks()
{
	int chunks = 0;
	int in_chunk = 0;
	struct mem_block_t* allocated_blk = find_block(kallocator.allocated_blocks, kallocator.memory);
	struct mem_block_t* free_blk = find_block(kallocator.free_blocks, kallocator.memory);
	void* next_blk_start = NULL;

	while(allocated_blk != NULL || free_blk != NULL){
		if(allocated_blk != NULL){
			if(in_chunk){
				in_chunk = 0;
			}
			next_blk_start = allocated_blk->start + size_of(allocated_blk);
		}
		else if(free_blk != NULL){
			if(!in_chunk){
				in_chunk = 1;
				chunks++;
			}
			next_blk_start = free_blk->start + size_of(free_blk);
		}
		allocated_blk = find_block(kallocator.allocated_blocks, next_blk_start);
		free_blk = find_block(kallocator.free_blocks, next_blk_start);
	}
	return chunks;
}

int calculate_smallest_chunk()
{
	int smallest_size;
	int size;
	struct mem_block_t* free_blk = kallocator.free_blocks;

	if(free_blk != NULL){
		smallest_size = size_of(free_blk);
	}

	while(free_blk != NULL){
		size = size_of(free_blk);
		if(size < smallest_size){
			smallest_size = size;
		}
		free_blk = free_blk->next;
	}
	return smallest_size;
}

int calculate_largest_chunk()
{
	int largest_size;
	int size;
	struct mem_block_t* free_blk = kallocator.free_blocks;

	if(free_blk != NULL){
		largest_size = size_of(free_blk);
	}

	while(free_blk != NULL){
		size = size_of(free_blk);
		if(size > largest_size){
			largest_size = size;
		}
		free_blk = free_blk->next;
	}
	return largest_size;
}

void print_statistics() 
{
	int allocated_size = kallocator.size - kallocator.free_memory;
	int allocated_chunks = calculate_allocated_chunks();
	int free_size = kallocator.free_memory;
	int free_chunks = calculate_free_chunks();
	int smallest_free_chunk_size = calculate_smallest_chunk();
	int largest_free_chunk_size = calculate_largest_chunk();

	// Calculate the statistics

	printf("Allocated size = %d\n", allocated_size);
	printf("Allocated chunks = %d\n", allocated_chunks);
	printf("Free size = %d\n", free_size);
	printf("Free chunks = %d\n", free_chunks);
	printf("Largest free chunk size = %d\n", largest_free_chunk_size);
	printf("Smallest free chunk size = %d\n", smallest_free_chunk_size);
}
