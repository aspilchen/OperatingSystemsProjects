#include "mem_block.h"
#include <stdlib.h>
#include <assert.h>

void mem_block_init(struct mem_block_t* blk)
{
	if(blk != NULL){
		blk->size = 0;
		blk->is_allocated = UNALLOCATED;
		blk->start = NULL;
		blk->next = NULL;
	}
}

int size_of(struct mem_block_t* blk)
{
	int size = 0;
	if(blk != NULL){
		size = blk->size;
	}
	return size;
}

void mem_block_add(struct mem_block_t** head, struct mem_block_t* blk)
{
	if(head != NULL && blk != NULL){
		blk->next = *head;
		*head = blk;
	}
}

void mem_block_remove(struct mem_block_t** head, struct mem_block_t* target)
{
	struct mem_block_t* iter;
	if(head != NULL && target != NULL){
		if(*head == target){
			*head = (*head)->next;
			return;
		}
		iter = *head;
		while(iter != NULL){
			if(iter->next == target){
				iter->next = target->next;
				return;
			}
			else{
				iter = iter->next;
			}
		}
	}
}

struct mem_block_t* find_block(struct mem_block_t* head, void* target)
{
	struct mem_block_t* blk = head;
	while(blk != NULL){
		if(blk->start == target){
			return blk;
		}
		else{
			blk = blk->next;
		}
	}
	return blk;
}

void destroy_block_list(struct mem_block_t* head)
{
	struct mem_block_t* iter = NULL;
	struct mem_block_t* trail = NULL;
	if(head != NULL){
		iter = head;
		while(iter != NULL){
			trail = iter;
			iter = iter->next;
			free(trail);
		}
	}
}
