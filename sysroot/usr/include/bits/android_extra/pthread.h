#pragma once

#include <pthread.h>

#ifdef __ANDROID_EXTRA

__BEGIN_DECLS
int pthread_barrierattr_init(pthread_barrierattr_t* _Nonnull __attr);
int pthread_barrierattr_destroy(pthread_barrierattr_t* _Nonnull __attr);
int pthread_barrierattr_getpshared(const pthread_barrierattr_t* _Nonnull __attr, int* _Nonnull __shared);
int pthread_barrierattr_setpshared(pthread_barrierattr_t* _Nonnull __attr, int __shared);

int pthread_barrier_init(pthread_barrier_t* _Nonnull __barrier, const pthread_barrierattr_t* _Nullable __attr, unsigned __count);
int pthread_barrier_destroy(pthread_barrier_t* _Nonnull __barrier);
int pthread_barrier_wait(pthread_barrier_t* _Nonnull __barrier);

int pthread_spin_destroy(pthread_spinlock_t* _Nonnull __spinlock);
int pthread_spin_init(pthread_spinlock_t* _Nonnull __spinlock, int __shared);
int pthread_spin_lock(pthread_spinlock_t* _Nonnull __spinlock);
int pthread_spin_trylock(pthread_spinlock_t* _Nonnull __spinlock);
int pthread_spin_unlock(pthread_spinlock_t* _Nonnull __spinlock);

#if defined(__USE_GNU)
int pthread_getaffinity_np(pthread_t __pthread, size_t __cpu_set_size, cpu_set_t* __cpu_set);
int pthread_setaffinity_np(pthread_t __pthread, size_t __cpu_set_size, const cpu_set_t* __cpu_set);
#endif

__END_DECLS

#endif
