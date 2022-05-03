#ifndef __RESOURCE_TABLE_
#define __RESOURCE_TABLE_
#define INIT_RESOURCE_TABLE_SIZE 4

void resource_table_init();
void resource_table_add(SmartLock* lock);
void resource_table_resize(size_t new_size);
unsigned int resource_table_generate_id();
void resource_table_destroy();
int resource_table_cycle_check(SmartLock* origin_resource, pthread_t origin_thread);
//int resource_table_cycle_check_helper(SmartLock* origin_resource, SmartLock* curr_resource, struct thread_node* search_root);

#endif
