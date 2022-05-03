//#include <errno.h>
#include <linux/uaccess.h>
#include "array_stats.h"

/**
 * stats: A pointer to one array_stats structure allocated by the user-space application. Structure will be written to by
 * the sys-call to store the minimum, maximum, and sum of all values in the array pointed to by data.
 * data: An array of long int values passed in by the user-space application.
 * size: The number of elements in data. Must be > 0.
 */
asmlinkage long sys_array_stats(
                struct array_stats *stats, 
                long data[], 
                long size)
{
	long result = 0;
	long i = 0;
	long data_buffer = 0;
	long min = 0;
	long max = 0;
	long sum = 0;
	size_t size_of_long = sizeof(long);

	if(size <= 0){
		printk("Size <= 0\n");
		return -EINVAL;
	}

	/*
	if(stats == NULL || data == NULL){
		return -EFAULT;
	}
	*/

	// Get stats[0].
	if(copy_from_user(&data_buffer, &(data[i]), size_of_long) != 0){
		printk("Segfault in data\n");
		return -EFAULT;
	}

	// Initialize stats from data[0].
	min = data_buffer;
	max = data_buffer;
	sum = data_buffer;

	// Analyse remaining data[].
	for(i = 1; i < size; i++){
		if(copy_from_user(&data_buffer, &(data[i]), size_of_long) != 0){
			printk("Segfault in data\n");
			return -EFAULT;
		}


		if(data_buffer < min){
			//printk("NEW MIN == %ld\n", data_buffer);
			min = data_buffer;
		}

		if(max < data_buffer){
			//printk("NEW MAX == %ld\n", data_buffer);
			max = data_buffer;
		}

		sum += data_buffer;
	}

	if( copy_to_user(&(stats->min), &min, sizeof(long)) != 0 ||
			copy_to_user(&(stats->max), &max, sizeof(long)) != 0 ||
			copy_to_user(&(stats->sum), &sum, sizeof(long)) != 0	)
	{
		printk("Segfault in stats\n");
		return -EFAULT;
	}

	return result;
}

