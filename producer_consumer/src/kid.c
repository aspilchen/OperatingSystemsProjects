/*****************************************************
 * kid.c                                             *
 *                                                   *
 * Functions to create and control kid threads.      *
 * Kid threads act as consumers to the bounded       *
 * buffer in bbuff.                                  *
 *                                                   *
 *****************************************************/

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "../headers/kid.h"
#include "../headers/candy_t.h"
#include "../headers/bbuff.h"
#include "../headers/stats.h"

#define MAX_WAIT 1

pthread_t *kid_threads; // Array of consumer threads.
int num_kids = 0;

/**
 * Initilize kid_threads array.
 * Create desired number of consumer threads.
 * n_kids: Desired number of consumer threads.
 */
void kid_init(const int n_kids)
{
	kid_threads = (pthread_t*) malloc(sizeof(pthread_t) * n_kids);

	for(int i = 0; i < n_kids; i++){
		kid_create_thread();
	}
}

/**
 * Consumes candy_t structs from bounded buffer in bbuff.
 * Saves candy information using stats.c for statistical data.
 * Meant to be run in seperate thread.
 * Runs indefinately until thread is cancelled.
 * arg: Unused, exists for thread purposes.
 */
void* kid_thread_function(void *arg)
{
	candy_t *candy;
	int wait_time = 0;
	double delay = 0;
	int f_num;
	double t_stamp;
	// Extract candy from bbuff.
	// Save data from candy in stats.
	// Deallocate candy_t struct.
	// Wait random number of seconds between 0 and MAX_WAIT
	while(true){
		wait_time = rand() % MAX_WAIT;
		candy = (candy_t*) bbuff_blocking_extract();
		f_num = candy->factory_number;
		t_stamp = candy->time_stamp_in_ms;
		delay = current_time_in_ms() - t_stamp;
		stats_record_consumed(f_num, delay);
		free(candy);
		sleep(wait_time);
	}
	return NULL;
}

/**
 * Creates single consumer thread.
 * Saves thread in kid_threads array.
 * Increments num_kids.
 */
pthread_t kid_create_thread()
{
	pthread_t thread;
	pthread_create(&thread, NULL, kid_thread_function, NULL);
	kid_threads[num_kids] = thread;
	num_kids++;
	return thread;
}

/**
 * Waits for for bbuff to empty.
 * Cancels all consumer threads.
 * Joins threads back into main program.
 * Deallocates kid_threads array.
 */
void kid_stop_threads()
{
	while(!bbuff_is_empty()){
		printf("Waiting for all candy to be consumed.\n");
	}

	printf("Stopping kids.\n");

	for(int i = 0; i < num_kids; i++){
		pthread_cancel(kid_threads[i]);
		pthread_join(kid_threads[i], NULL);
	}

	free(kid_threads);
}
