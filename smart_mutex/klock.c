#include "klock.h"
#include "resource_table.h"
#include "thread_dll.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Initialize Smartlock.
 * Adds lock to resource table.
 * lock: Smartlock to be initialized.
 */
void init_lock(SmartLock* lock)
{
	unsigned int id = resource_table_generate_id();
	struct thread_dll* dll = (struct thread_dll*) malloc(sizeof(struct thread_dll));
	if(dll != NULL){
		pthread_mutex_init(&(lock->mutex), NULL);
		thread_dll_init(dll);
		lock->id = id;
		lock->requests = dll;
		resource_table_add(lock);
		lock->being_cycle_checked = 0;
	}
}

/**
 * Mutex lock with deadlock detection.
 * lock: Smartlock to be locked.
 * returns 1 if request is safe, 0 otherwise.
 */
int lock(SmartLock* lock)
{
	pthread_mutex_lock(&(lock->self_mutex));
	pthread_t current_thread = pthread_self();
	struct thread_dll* dll = lock->requests;
	int creates_cycle = resource_table_cycle_check(lock, current_thread);
	if(!creates_cycle){
		//printf("NO CYCLE FOUND\n");
		thread_dll_add(dll, current_thread);
		pthread_mutex_unlock(&(lock->self_mutex));
		pthread_mutex_lock(&(lock->mutex));
		return 1;
	}
	printf("CYCLE DETECTED -- mutex denied\n");
	pthread_mutex_unlock(&(lock->self_mutex));
	return 0;
}

/**
 * Unlocks mutex.
 * Removes pthread from lock requests.
 * lock: Smartlock to be unlocked.
 */
void unlock(SmartLock* lock)
{
	struct thread_dll* dll = lock->requests;
	pthread_t thread = pthread_self();
	thread_dll_remove(dll, thread);
	pthread_mutex_unlock(&(lock->mutex));
}

/**
 * Destroys requests member.
 * Destroys mutex and self_mutex members.
 * lock: Smartlock to be destroyed.
 */
void SmartLock_destroy(SmartLock* lock)
{
	thread_dll_destroy(lock->requests);
	pthread_mutex_destroy(&(lock->mutex));
	pthread_mutex_destroy(&(lock->self_mutex));
}

/*
 * Cleanup any dynamic allocated memory for SmartLock to avoid memory leak
 * You can assume that cleanup will always be the last function call
 * in main function of the test cases.
 */
void cleanup()
{
	resource_table_destroy();
}
