/*****************************************************
 * candy_t.c                                         *
 *                                                   *
 * Functions to create and interact with candy_t     *
 * structs.                                          *
 *                                                   *
 *****************************************************/

#include <stdlib.h>
#include <time.h>
#include "../headers/bbuff.h"
#include "../headers/candy_t.h"

/**
 * Returns current time in milliseconds.
 */
double current_time_in_ms(void)
{
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return now.tv_sec * 1000.0 + now.tv_nsec/1000000.0;
}

/**
 * Allocates memory for a candy_t struct and returns pointer.
 * Sets candy_t factory_number.
 * Sets candy_t time stamp to current time in milliseconds.
 * f_number: Factory number to assign to candy_t.
 */
candy_t* candy_create(const int f_number)
{
	candy_t *candy = (candy_t*) malloc(sizeof(candy_t));
	candy->factory_number = f_number;
	candy->time_stamp_in_ms = current_time_in_ms();
	return candy;
}
