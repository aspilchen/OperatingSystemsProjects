#ifndef __KLOCK_H__
#define __KLOCK_H__

#include <pthread.h>

typedef struct {
	unsigned int id;
	int being_cycle_checked;
	pthread_mutex_t mutex;
	pthread_mutex_t self_mutex;
	struct thread_dll* requests;
} SmartLock;

void init_lock(SmartLock* lock);
int lock(SmartLock* lock);
void unlock(SmartLock* lock);
void SmartLock_destroy(SmartLock* lock);
void cleanup();

#endif
