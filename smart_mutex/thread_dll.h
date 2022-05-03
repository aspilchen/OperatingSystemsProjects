#ifndef __THREAD_QUEUE_
#define __THREAD_QUEUE_

#include <pthread.h>

struct thread_node {
	pthread_t thread;
	struct thread_node* next;
	struct thread_node* previous;
};

struct thread_dll {
	pthread_mutex_t mutex;
	struct thread_node* head;
};

void thread_dll_init(struct thread_dll* dll);
void thread_dll_add(struct thread_dll* dll, pthread_t thread);
void thread_dll_remove(struct thread_dll* dll, pthread_t target);
struct thread_node* thread_dll_search(struct thread_dll* dll, pthread_t target);
void thread_dll_destroy(struct thread_dll* dll);

void thread_node_init(struct thread_node* node);
void thread_node_destroy(struct thread_node* node);

#endif
