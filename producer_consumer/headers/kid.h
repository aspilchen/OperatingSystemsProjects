#ifndef KID
#define KID

#include <pthread.h>

void kid_init(const int n_kids);

void* kid_thread_function(void *arg);

pthread_t kid_create_thread();

void kid_stop_threads();

#endif
