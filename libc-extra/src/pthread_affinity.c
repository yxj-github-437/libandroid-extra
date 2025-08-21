#define _GNU_SOURCE

#include <errno.h>
#include <pthread.h>
#include <sched.h>


int pthread_getaffinity_np(pthread_t __pthread, size_t __set_size, cpu_set_t* __set)
{
    pid_t tid = pthread_gettid_np(__pthread);
    if (tid == -1) return ESRCH;
    if (sched_getaffinity(tid, __set_size, __set) == -1)
        return errno;
    return 0;
}

int pthread_setaffinity_np(pthread_t __pthread, size_t __set_size, const cpu_set_t* __set)
{
    pid_t tid = pthread_gettid_np(__pthread);
    if (tid == -1) return ESRCH;
    if (sched_setaffinity(tid, __set_size, __set) == -1)
        return errno;
    return 0;
}
