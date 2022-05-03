/*****************************************************
 * stats.c                                           *
 *                                                   *
 * Functions to save and process statistacal data    *
 * from candy_t structs.                             *
 *                                                   *
 *****************************************************/

#include <stdio.h>
#include  <stdlib.h>
#include <string.h>
#include <limits.h>
#include "../headers/stats.h"

int n_producers = 0;
int *produced;
int *consumed;
double *min_delay;
double *max_delay;
double *total_delay;

/**
 * Initialize arrays to store stats data.
 * Each array should contain the same number of elements as there are producer threads.
 * num_producer: Number of producer threads being created.
 */
void stats_init(int num_producers)
{
	n_producers = num_producers;
	produced = (int*)calloc(num_producers, sizeof(int));
	consumed = (int*)calloc(num_producers, sizeof(int));
	min_delay = (double*)malloc(sizeof(double) * num_producers);
	max_delay = (double*)malloc(sizeof(double) * num_producers);
	total_delay = (double*)malloc(sizeof(double) * num_producers);
	for(int i = 0; i < n_producers; i++){
		min_delay[i] = (double) 999999;
		max_delay[i] = (double) 0;
		total_delay[i] = (double) 0;
	}
}

/**
 * Dealocates arrays.
 */
void stats_cleanup(void)
{
	free(produced);
	free(consumed);
	free(min_delay);
	free(max_delay);
	free(total_delay);
}

/**
 * Increment record containing production info for factory.
 * factory_number: Factory that produced candy.
 */
void stats_record_produced(int factory_number)
{
	produced[factory_number]++;
}

/**
 * Increment consumption record for factory that produced.
 * Save the time taken to consume candy_t after it was created.
 * factory_number: Number of candy_t producer thread.
 * delay_in_ms: Length of time between candies creation and consumption in millisectons.
 */
void stats_record_consumed(int factory_number, double delay_in_ms)
{
	consumed[factory_number]++;
	total_delay[factory_number] += delay_in_ms;
	if(delay_in_ms < min_delay[factory_number]){
		min_delay[factory_number] = delay_in_ms;
	}
	if(delay_in_ms > max_delay[factory_number]){
		max_delay[factory_number] = delay_in_ms;
	}
}

/**
 * Display statistical data.
 */
void stats_display(void)
{
	printf("%8s%8s%8s%15s%15s%15s\n", "Factory#", "#Made", "#Eaten", "Min Delay[ms]", "Avg Delay[ms]", "Max Delay[ms]");
	double avg_delay;
	for(int i = 0; i < n_producers; i++){
		if(produced[i] != consumed[i]){
			printf("ERROR: Mismatch between number made and eaten.\n");
		}
		else{
			avg_delay = total_delay[i] / produced[i];
			printf("%8d%8d%8d%15.5f%15.5f%15.5f\n", i, produced[i], consumed[i], min_delay[i], avg_delay, max_delay[i]);
		}
	}
}
