#ifndef ThreadPool_h
#define ThreadPool_h

#include "../DataStructures/Lists/Queue.h"

#include <pthread.h>

// DATA TYPES
struct ThreadJob
{
    void * (*job)(void *arg);
    void *arg;
};

struct ThreadPool
{
    int num_threads;
    int active; // is active
    struct Queue work;
    pthread_t *pool; // pool of threads
    pthread_mutex_t lock; // protect resources from being accessed simultaneously by multiple threads
    pthread_cond_t signal; // synchronize threads; wait for conditions before continuing execution
    void (*add_work)(struct ThreadPool *thread_pool, struct ThreadJob thread_job);
};

// CONSTRUCTORS
struct ThreadPool thread_pool_constructor(int num_threads);
struct ThreadJob thread_job_constructor(void * (*job)(void *arg), void *arg);

void thread_pool_destructor(struct ThreadPool *thread_pool);
#endif 
