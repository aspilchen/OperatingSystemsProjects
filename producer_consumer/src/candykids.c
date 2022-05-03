/*****************************************************
 * candykids.c                                       *
 *                                                   *
 * Contains main function for candykids program.     *
 * Demonstrates the use of semaphores to control     *
 * thread synchronization to solve the 							 *
 * producer/consumer buffer problem.                 *
 *                                                   *
 *****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../headers/candy_t.h"
#include "../headers/candy_factory.h"
#include "../headers/bbuff.h"
#include "../headers/kid.h"
#include "../headers/stats.h"

#define N_ARGS 4

/**
 * Prints error message and exits program.
 */
void handle_argument_error()
{
	printf("Error: All arguments must be positive.\n");
	exit(EXIT_FAILURE);
}

/**
 * Main function.
 * Uses command line arguments to determine the number of producer and consumer threads are created.
 * Uses command line arguments to determine roughly how long the threads run in seconds.
 * Initializes threads.
 * Destroys threads on program end.
 */
int main(int argc, char *argv[]) 
{

	int number_factories;
	int number_kids;
	int number_seconds;

	// Set random seed for random thread wait times.
	srand(time(NULL)); 

	// Extract and verify program arguments.
	if(argc != N_ARGS){
		handle_argument_error();
	}
	
	number_factories = atoi(argv[1]);
	number_kids = atoi(argv[2]);
	number_seconds = atoi(argv[3]);
		
	if(number_factories <= 0 || number_kids <= 0 || number_seconds <= 0){
		handle_argument_error();
	}

	// Initialize bounded buffer.
	// Initialize statistics module.
	// Initialize producer threads.
	// Initialize consumer threads.
	bbuff_init();
	stats_init(number_factories);
	candy_factory_init(number_factories);
	kid_init(number_kids);

	// Count elapsed time in seconds.
	for(int i = 0; i < number_seconds; i++){
		printf("Time  %is:\n", i);
		sleep(1);
	}

	// Terminate producer threads.
	// Terminate consumer threads.
	candy_factory_stop_threads();
	kid_stop_threads();

	// Display stats info.
	// Deallocate stats info created on stats_init()
	stats_display();
	stats_cleanup();

	return EXIT_SUCCESS;
}
