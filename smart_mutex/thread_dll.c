#include "thread_dll.h"
#include <stdlib.h>
#include <pthread.h>
//#include <stdio.h>

/**
 * Initialize doubly linked list.
 * dll: Pointer to list being initialized.
 */
void thread_dll_init(struct thread_dll* dll)
{
	if(dll != NULL){
		pthread_mutex_init(&(dll->mutex), NULL);
		dll->head = NULL;
	}
}

/**
 * Add thread to front of doubly linked list.
 * dll: Pointer to list being modified.
 * thread: long unsigned int thread ID being added to resource.
 */
void thread_dll_add(struct thread_dll* dll, pthread_t thread)
{
	pthread_mutex_lock(&(dll->mutex));
	struct thread_node* node = NULL;
	struct thread_node* head = NULL;

	if(dll != NULL){
		// Create node containing thread to be added to list.
		node = (struct thread_node*) malloc(sizeof(struct thread_node));
		head = dll->head;
		if(node != NULL){
			// Set node links.
			// Insert node to head of list.
			thread_node_init(node);
			node->thread = thread;
			node->next = head;
			dll->head = node;
		}
		// If list was not empty, set old head->previous to point to new head.
		if(head != NULL){
			head->previous = node;
		}
	}

	pthread_mutex_unlock(&(dll->mutex));
}

/**
 * Remove thread from doubly linked list.
 * dll: Pointer to list being modified.
 * target: Thread to be removed.
 */
void thread_dll_remove(struct thread_dll* dll, pthread_t target)
{
	pthread_mutex_lock(&(dll->mutex)); // To ensure multiple removes do not happen in same resource at once.
	struct thread_node* node = NULL;
	struct thread_node* next = NULL;
	struct thread_node* prev = NULL;
	if(dll != NULL){
		node = thread_dll_search(dll, target); // Locate node containing target if exists.
		if(node != NULL){
			next = node->next;
			prev = node->previous;
			if(next != NULL){
				next->previous = prev;
			}
			if(prev != NULL){
				prev->next = next;
			}
			else{
				dll->head = next;
			}
			free(node);
		}
	}
	pthread_mutex_unlock(&(dll->mutex));
}

/**
 * Locates node containing target thread if exists.
 * dll: Doubly linked list to be searched.
 * target: Thread to search for.
 * returns: thread_node containing target thread if thread is found, NULL otherwise.
 */
struct thread_node* thread_dll_search(struct thread_dll* dll, pthread_t target)
{
	struct thread_node* node = NULL;
	if(dll != NULL){
		node = dll->head;
		// Iterate through list until target or end of list is found.
		while(node != NULL && node->thread != target){
			node = node->next;
		}
	}
	return node;
}

/**
 * Frees all nodes in doubly linked list.
 * Frees list.
 * dll: List to destroy.
 */
void thread_dll_destroy(struct thread_dll* dll)
{
	struct thread_node* node = NULL;
	struct thread_node* trail = NULL;

	if(dll != NULL){
		trail = dll->head;

		// Destroy all nodes in dll.
		while(trail != NULL){
			node = trail->next;
			thread_node_destroy(trail);
			trail = node;
		}
	}

	pthread_mutex_destroy(&(dll->mutex));
	free(dll);
}

/**
 * Initializes thread_node values to NULL.
 * node: node to be initialized.
 */
void thread_node_init(struct thread_node* node)
{
	if(node != NULL){
		node->next = NULL;
		node->previous = NULL;
	}
}

/**
 * Frees thread_node.
 * node: Node to be destroyed.
 */
void thread_node_destroy(struct thread_node* node)
{
	if(node != NULL){
		free(node);
	}
}
