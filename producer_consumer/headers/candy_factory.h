#ifndef CANDY_FACTORY
#define CANDY_FACTORY

#include <pthread.h>


void candy_factory_init(const int n_factories);

void* candy_factory_thread_function(void* arg);

pthread_t candy_factory_create_thread();

void candy_factory_stop_threads();

#endif
