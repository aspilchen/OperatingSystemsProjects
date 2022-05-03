#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include "process_ancestors.h"

#define _PROCESS_ANCESTORS_ 342

int do_syscall(struct process_info *info_array, const long size, long *num_filled);
int do_syscall_working(const long size);
void simple_test();

static int syscall_test_num = 0;

int main()
{
	simple_test();
	return 0;
}

void simple_test()
{
	const long size = 6;
	struct process_info info_array[size];
	long num_filled = 0;
	
	do_syscall(info_array, size, &num_filled);

	for(int i = 0; i < num_filled; i++){
		printf("\n-- TASK --\n");
		printf("pid = %ld\n", info_array[i].pid);
		printf("name = %s\n",  info_array[i].name);
		printf("state = %ld\n", info_array[i].state);
		printf("uid = %ld\n", info_array[i].uid);
		printf("nvcsw = %ld\n", info_array[i].nvcsw);
		printf("nivcsw = %ld\n", info_array[i].nivcsw);
	}
}


int do_syscall(struct process_info *info_array, const long size, long *num_filled)
{
	int result;

	//int my_errno = errno;

	printf("\nTest %d: ..Diving to kernel level\n", syscall_test_num);
	result = syscall(_PROCESS_ANCESTORS_, info_array, size, num_filled);
	printf("..Rising to user level w/ result = %d\n", result);

	return result;
}

int do_syscall_working(const long size)
{
	struct process_info info_array[size];
	long num_filled = 0;

	int result = do_syscall(info_array, size, &num_filled);

	assert(result == 0);

	return result;
}
