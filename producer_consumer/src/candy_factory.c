/*****************************************************
 * candy_factory.c                                   *
 *                                                   *
 * Creates and controls threads that produce candy_t *
 * objects. Saves those objects in bbuff.c bounded   *
 * buffer.                                           *
 *                                                   *
 *****************************************************/
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "../headers/candy_t.h"
#include "../headers/bbuff.h"
#include "../headers/candy_factory.h"
#include "../headers/stats.h"

#define FACTORY_MAX_WAIT_SECONDS 3

pthread_t *factory_threads; // Array to store factory threads.
int num_factories = 0;

_Bool stop_factory_threads = false; // Thread control flag. Threads stop when true.

/**
 * Initializes array to hold factory threads.
 * Creates desired number of candy_factory producer threads.
 * n_factories: Desired number of threads.
 */
void candy_factory_init(const int n_factories)
{
	num_factories = 0;
	factory_threads = (pthread_t*)malloc(sizeof(pthread_t) * n_factories);

	for(int i = 0; i < n_factories; i++){
		candy_factory_create_thread();
	}
}

/**
 * Produces candy_t objects containing threads factory number and current time in ms.
 * Saves candy in bounded buffer located in bbuff.c
 * Waits a maximum number of FACTORY_MAX_WAIT_SECONDS seconds.
 * Produces indefinately until stop_factory_threads = true.
 * arg: Pointer to integer representing factory number.
 */
void* candy_factory_thread_function(void* arg)
{
	int* factory_number = ((int*) arg);
	int wait_time = 0;
	candy_t *candy;
	// Produce candy until stop_factory_threads = true.
	// Saves candy in bounded buffer.
	// Waits.
	while (!stop_factory_threads){
		wait_time = rand() % FACTORY_MAX_WAIT_SECONDS;
		candy = candy_create(*factory_number);
		stats_record_produced(*factory_number);
		bbuff_blocking_insert(candy);
		printf("\tFactory %i ship candy & wait %is\n", *factory_number, wait_time);
		sleep(wait_time);
	}

	// Free factory_number and exit thread.
	printf("Candy-factory %i done\n", *factory_number);
	free(factory_number);
	return NULL;
}

/**
 * Create single factory thread.
 * Allocates integer to pass to thread as factory number.
 * Saves thread in factory_threads array.
 */
pthread_t candy_factory_create_thread()
{
	int* f_number = (int*)malloc(sizeof(int));
	*f_number = num_factories;
	pthread_t thread;
	pthread_create(&thread, NULL, candy_factory_thread_function, f_number);
	factory_threads[num_factories] = thread;
	num_factories++;
	return thread;
}

/**
 * Halts production in all threads.
 * Joins threads back to main program.
 * Frees factory_thread array.
 */
void candy_factory_stop_threads()
{
	printf("Stopping candy factories...\n");
	stop_factory_threads = true;

	for(int i = 0; i < num_factories; i++){
		pthread_join(factory_threads[i], NULL);
	}

	free(factory_threads);
}
