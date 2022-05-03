#include <linux/sched.h>
#include <linux/uaccess.h>
#include "process_ancestors.h"

/**
 * Prototype
 */
static long copy_task_info(struct process_info *pi, struct task_struct *task);

/**
 * System call that gathers information of current process and its parents and copies that data to an array of user
 * leves process_info structs.
 *
 * ..................................................
 * PROCESS FIELDS COPIED:
 * pid                     * Process ID
 * comm                    * Program name of process
 * state                   * Current process state
 * uid                     * User ID of process owner
 * nvcsw                   * # voluntary context switches
 * nivcsw                  * # involuntary context switches
 * num_children            * # children process has
 * num_siblings            * # sibling process has
 * ..................................................
 *
 * info_array[]: An array of process_info structs that will be written to by the kernel as it fills in information from 
 * the current process on up through its ancestors.
 *
 * size: The number of structs in info_array[]. This is the maximum number of structs that the kernel will write into
 *	the array (starting with the current process as the first entry and working up from there). The size may be larger
 *	or smaller than the actual number of ancestors of the current process: larger means some entries are left unused
 *	(see num_filled); smaller means information about some processes not written into the array.
 *
 * num_filled: A pointer to a long. To this location the kernel will store the number of structs (in info_array[])
 *	 which are written by the kernel. May be less than size if the number of ancestors for the current process is less
 *	 than size.
 */
asmlinkage long sys_process_ancestors(
                struct process_info info_array[], 
                long size, 
                long *num_filled)
{
	long result = 0;
	struct task_struct *task = get_current();
	long num_process = 0;

	if(size <= 0){
		return -EINVAL;
	}

	// Iterate through current process and all parents.
	// Copy process data into_array[num_process].
	// Increment num_process.
	while(task->parent != task && num_process < size){

		result = copy_task_info(&(info_array[num_process]), task);	// Kernal level to user level copy.
		// If copy fails, halt and return result.
		if(result != 0){
			break;
		}

		num_process++;
		task = task->parent;
	}

	// Copy number processes analyzed from kernal level to user level.
	if(copy_to_user(num_filled, &num_process, sizeof(long)) != 0){
		result = -EFAULT;
	}
	
	return result;
}

/*************************************************************
 * Functions to copy relevent data into user space.
 *************************************************************/

/**
 * Copies # involuntary context switches of process from kernal level task_struct to user level process_info->nivscw.
 * 
 * *pi: User level process_info to be the copy destination.
 * *task: Kernal level task_struct copy source.
 */
static long copy_task_nivcsw(struct process_info *pi, struct task_struct *task)
{
	long result = 0;
	long nivcsw = task->nivcsw;	// # involuntary context switches

	if(copy_to_user(&(pi->nivcsw), &nivcsw, sizeof(long)) != 0){
		result = -EFAULT;
	}

	return result;
}

/**
 * Copies # voluntary context switches of process from kernal level task_struct to user level process_info->nvscw.
 * 
 * *pi: User level process_info to be the copy destination.
 * *task: Kernal level task_struct copy source.
 */
static long copy_task_nvcsw(struct process_info *pi, struct task_struct *task)
{
	long result = 0;
	long nvcsw = task->nvcsw;	// # voluntary context switches

	if(copy_to_user(&(pi->nvcsw), &nvcsw, sizeof(long)) != 0){
		result = -EFAULT;
	}

	return result;
}

/**
 * Copies process user ID from kernal level task_struct into user level process_info.
 *
 * *pi: User level process_info to be the copy destination.
 * *task: Kernal level task_struct copy source.
 */
static long copy_task_uid(struct process_info *pi, struct task_struct *task)
{
	long result = 0;
	const struct cred *credentials = task->cred;
	kuid_t buff = credentials->uid;
	long uid = buff.val;	// User ID.

	if(copy_to_user(&(pi->uid), &uid, sizeof(long)) != 0){
		result = -EFAULT;
	}

	return result;
}

/**
 * Copies current process state from kernal level task_struct into user level process_info->state.
 *
 * *pi: User level process_info to be the copy destination.
 * *task: Kernal level task_struct copy source.
 */
static long copy_task_state(struct process_info *pi, struct task_struct *task)
{
	long result = 0;
	long state = task->state;

	if(copy_to_user(&(pi->state), &state, sizeof(long)) != 0){
		result = -EFAULT;
	}

	return result;
}

/**
 * Copies process command name from kernal level task_struct into user level process_info->name.
 *
 * *pi: User level process_info to be the copy destination.
 * *task: Kernal level task_struct copy source.
 */
static long copy_task_name(struct process_info *pi, struct task_struct *task)
{
	long result = 0;

	// Copy directly, no buffer used.
	if(copy_to_user(&(pi->name), task->comm, ANCESTOR_NAME_LEN) != 0){
		result = -EFAULT;
	}

	return result;
}

/**
 * Copies process ID from kernal level task_struct into user level process_info->pid
 *
 * *pi: User level process_info to be the copy destination.
 * *task: Kernal level task_struct copy source.
 */
static long copy_task_pid(struct process_info *pi, struct task_struct *task)
{
	long result = 0;
	long pid = task->pid;	// Process ID.

	if(copy_to_user(&(pi->pid), &pid, sizeof(long)) != 0){
		result = -EFAULT;
	}

	return result;
}

/**
 * Copies all relevant process information from kernal level task_struct into user level process_info.
 *
 * *pi: User level process_info to be the copy destination.
 * *task: Kernal level task_struct copy source.
 */
static long copy_task_info(struct process_info *pi, struct task_struct *task)
{
	long result = 0;

	// If any copy fails, halt and return -EFAULT.
	if( copy_task_pid(pi, task) != 0 ||
			copy_task_name(pi, task) != 0 ||
			copy_task_state(pi, task) != 0 ||
			copy_task_uid(pi, task) != 0 ||
			copy_task_nvcsw(pi, task) != 0 ||
			copy_task_nivcsw(pi, task) != 0 )
	{
		result = -EFAULT;
	}

	return result;
}
