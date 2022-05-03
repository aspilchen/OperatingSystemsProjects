#include <stdlib.h>
#include "klock.h"
#include "thread_dll.h"
#include "resource_table.h"
#include <stdio.h>

SmartLock** resource_table = NULL;
pthread_mutex_t resource_table_mutex;
size_t resource_table_size = 0;
size_t n_resources = 0;

/**
 * Initialize resource table to size INIT_RESOURCE_TABLE_SIZE.
 */
void resource_table_init()
{
	resource_table_resize(INIT_RESOURCE_TABLE_SIZE);
	if(resource_table != NULL){
		resource_table_size = INIT_RESOURCE_TABLE_SIZE;
	}
}

/**
 * Resizes resource table using realloc().
 * nmemb: Number of resources the new array will contain.
 */
void resource_table_resize(size_t nmemb)
{
	resource_table_size = nmemb;
	resource_table = (SmartLock**) realloc(resource_table, nmemb * sizeof(SmartLock*));
}

/**
 * Adds a SmartLock to the resource table.
 * Expands table size by 50% if table is full.
 * lock: Pointer to SmartLock to be added.
 */
void resource_table_add(SmartLock* lock)
{
	size_t new_size;
	// Init. table if table is uninitialized.
	if(resource_table == NULL){
		resource_table_init();
	}
	// Expand table if full.
	else if(n_resources == resource_table_size){
		new_size = resource_table_size + (resource_table_size/2);
		resource_table_resize(new_size);
	}
	// Add lock to table.
	resource_table[lock->id] = lock;
	n_resources++;
}

/**
 * Generates an ID for a new resource that will be that resources index in table.
 */
unsigned int resource_table_generate_id()
{
	return n_resources;
}

/**
 * Removes SmartLock resource from the resource table.
 * lock: SmartLock to be removed.
 */
void resource_table_remove(SmartLock* lock)
{
	size_t i = 0;
	if(n_resources > 0){
		// Locate lock to be removed.
		while(i < n_resources && lock != resource_table[i]){
			i++;
		}
		// If lock is located, shift remaining resources left by one index.
		if(i < n_resources && lock == resource_table[i]){
			for(; i < n_resources-1; i++){
				resource_table[i] = resource_table[i+1];
			}
			// Decrement number of resources in table.
			n_resources--;
		}
	}
}

/**
 * Recursive helper to detect cycles in resource_table.
 * Traces nodes through resource_table and returns 1 if it visits origin_resource.
 * origin_resource: Starting point of original search.
 * curr_resource: Current resource being search.
 * search_root: Thread node to start search from.
 * returns 1 if cycle is found, 0 otherwise.
 */
int resource_table_cycle_check_helper(SmartLock* origin_resource, SmartLock* curr_resource, struct thread_node* search_root)
{
	if(curr_resource == origin_resource){
		curr_resource->being_cycle_checked = 0;
		return 1;
	}
	curr_resource->being_cycle_checked = 1; // Flag to indicate this resource is already being searched.
	SmartLock* lock;
	struct thread_dll* dll;
	struct thread_node* node;
	struct thread_node* next = search_root->next;
	struct thread_node* previous = search_root->previous;

	// Check recursively down the linked list.
	while(next != NULL){
		// Locate thread id in resource NOT being checked already.
		// Recursively call self for rest of request nodes in that resource.
		for(size_t i = 0; i < n_resources; i++){
			lock = resource_table[i];
			if(lock->being_cycle_checked != 1){
				dll = lock->requests;
				node = thread_dll_search(dll, next->thread);
				if(node != NULL){
					if(resource_table_cycle_check_helper(origin_resource, lock, node)){
						// Cycle has been located
						curr_resource->being_cycle_checked = 0;
						return 1;
					}
				}
			}
		}
		next = next->next;
	}

	// Check recursively up the linked list.
	while(previous != NULL){
		// Locate thread id in resource NOT being checked already.
		// Recursively call self for rest of request nodes in that resource.
		for(size_t i = 0; i < n_resources; i++){
			lock = resource_table[i];
			if(!lock->being_cycle_checked){
				dll = lock->requests;
				node = thread_dll_search(dll, previous->thread);
				if(node != NULL){
					if(resource_table_cycle_check_helper(origin_resource, lock, node)){
						// Cycle has been located.
						curr_resource->being_cycle_checked = 0;
						return 1;
					}
				}
			}
		}
		previous = previous->previous;
	}
	 // No cycle has been found.
	curr_resource->being_cycle_checked = 0;
	return 0;
}

/**
 * Preemptively checks for cycles in table.
 * origin_resource: Resource requesting the cycle check.
 * thread: thread requesting the cycle check.
 * returns: 1 if request will create cycle, 0 otherwise.
 */
int resource_table_cycle_check(SmartLock* origin_resource, pthread_t thread)
{
	pthread_mutex_lock(&resource_table_mutex); // Only one resource may ask for cycle check at a time.
	SmartLock* lock;
	struct thread_dll* requests;
	struct thread_node* node;
	int cycle_detected;
	// Search for thread in other resources request lists.
	// Recursively traverse adjacencies to search for cycle if thread is found.
	for(size_t i = 0; i < n_resources; i++){
		lock = resource_table[i];
		if(lock != origin_resource){
			requests = lock->requests;
			node = thread_dll_search(requests, thread); // Search for thread in lock.
			if(node != NULL){
				// Recursively check if request will create cycle.
				cycle_detected = resource_table_cycle_check_helper(origin_resource, lock, node);
				if(cycle_detected){
					// Cycle detected.
					pthread_mutex_unlock(&resource_table_mutex);
					return 1;
				}
			}
		}
	}
	// Cycle is not found.
	pthread_mutex_unlock(&resource_table_mutex);
	return 0;
}


/**
 * Destroys all resources.
 * Deallocated resource_table.
 */
void resource_table_destroy()
{
	for(size_t i = 0; i < n_resources; i++){
		SmartLock_destroy(resource_table[i]);
	}
	free(resource_table);
	pthread_mutex_destroy(&resource_table_mutex);
	resource_table = NULL;
}
