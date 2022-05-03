/*****************************************************
 * bbuff.c                                           *
 *                                                   *
 * Bounded buffer using semaphores to control thread *
 * synchronization.                                  *
 *                                                   *
 *****************************************************/
#include <stdlib.h>
#include <stdbool.h>
#include <semaphore.h>
#include "../headers/bbuff.h"

#define BUFFER_SIZE 10

void *bbuff[BUFFER_SIZE]; // Queue of size BUFFER_SIZE implemented as circular array.
int bbuff_start;
int bbuff_end;
int bbuff_size;

sem_t mutex; // Mutex lock for insertion and extraction.
sem_t full;  // Control maximum buffer bound.
sem_t empty; // Control minumum buffer bound.

/**
 * Initialize buffer and samaphores.
 */
void bbuff_init(void)
{
	sem_init(&mutex, 0, 1);
	sem_init(&empty, 0, BUFFER_SIZE);
	sem_init(&full, 0, 0);
	bbuff_start = bbuff_end = bbuff_size = 0;
}

/**
 * Insert void pointer into buffer. Uses semaphores to control access for multiple threads.
 * Increments bbuff_size.
 * item: Pointer to be inserted.
 */
void bbuff_blocking_insert(void* item)
{
	sem_wait(&empty); // Ensure buffer not full.
	sem_wait(&mutex); // Ensure no threads in critical section.
	// Critical section.
	bbuff[bbuff_end] = item;
	bbuff_end = (bbuff_end + 1) % BUFFER_SIZE;
	bbuff_size++;
	// End critical section.
	sem_post(&mutex); 
	sem_post(&full);
}

/**
 * Extract next void pointer from buffer. Uses semaphores to control acces for multiple threads.
 * Decrements bbuff_size.
 */
void* bbuff_blocking_extract(void)
{
	void *item = NULL;

	sem_wait(&full);  // Ensure buffer not empty.
	sem_wait(&mutex); // Ensure no threads in critical section.
	// Critical section.
	item = bbuff[bbuff_start];
	bbuff_start = (bbuff_start + 1) % BUFFER_SIZE;
	bbuff_size--;
	// End critical section.
	sem_post(&mutex);
	sem_post(&empty);

	return item;
}

/**
 * Returns true if buffer is empty, false otherwise.
 */
_Bool bbuff_is_empty(void)
{
	return bbuff_size == 0;
}

/**
 * Deallocate contents of buffer.
 * Deallocate buffer
 * Destroy semaphores.
 */
void bbuff_destroy(void)
{
	void *item = NULL;
	while(!bbuff_is_empty()){
		item = bbuff_blocking_extract();
		free(item);
	}
	sem_destroy(&mutex);
	sem_destroy(&full);
	sem_destroy(&empty);
}

