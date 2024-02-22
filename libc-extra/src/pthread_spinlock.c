/*
 * Copyright (C) 2015 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include "bionic_futex.h"
#include <assert.h>   // assert_static
#include <pthread.h>
#include <stdalign.h> // alignof
#include <stdatomic.h>
#include <stdbool.h>

// Lock is used in places like pthread_rwlock_t, which can be initialized without calling
// an initialization function. So make sure Lock can be initialized by setting its memory to 0.
enum LockState
{
    Unlocked = 0,
    LockedWithoutWaiter,
    LockedWithWaiter,
};
struct Lock
{
    _Atomic(enum LockState) state;
    bool process_shared;
};

static inline __always_inline
void Lock_init(struct Lock* lock, bool process_shared)
{
    atomic_init(&lock->state, Unlocked);
    lock->process_shared = process_shared;
}

static inline __always_inline
bool Lock_trylock(struct Lock* lock)
{
    enum LockState old_state = Unlocked;
    return __predict_true(atomic_compare_exchange_strong_explicit(
        &lock->state, &old_state, LockedWithoutWaiter, memory_order_acquire, memory_order_relaxed));
}

static inline __always_inline
void Lock_lock(struct Lock* lock)
{
    enum LockState old_state = Unlocked;
    if (__predict_true(atomic_compare_exchange_strong_explicit(&lock->state,
                                                               &old_state,
                                                               LockedWithoutWaiter,
                                                               memory_order_acquire,
                                                               memory_order_relaxed))) {
        return;
    }
    while (atomic_exchange_explicit(&lock->state, LockedWithWaiter, memory_order_acquire)
           != Unlocked) {
        // TODO: As the critical section is brief, it is a better choice to spin a few times
        // befor sleeping.
        __futex_wait_ex(&lock->state, lock->process_shared, LockedWithWaiter);
    }
    return;
}

static inline __always_inline
void Lock_unlock(struct Lock* lock)
{
    bool shared = lock->process_shared; /* cache to local variable */
    if (atomic_exchange_explicit(&lock->state, Unlocked, memory_order_release)
        == LockedWithWaiter) {
        // The Lock object may have been deallocated between the atomic exchange and the futex
        // wake call, so avoid accessing any fields of Lock here. In that case, the wake call
        // may target unmapped memory or trigger a spurious futex wakeup. The same situation
        // happens with pthread mutexes. References:
        //  - https://lkml.org/lkml/2014/11/27/472
        //  - http://austingroupbugs.net/view.php?id=811#c2267
        __futex_wake_ex(&lock->state, shared, 1);
    }
}

__BEGIN_DECLS

// User-level spinlocks can be hazardous to battery life on Android.
// We implement a simple compromise that behaves mostly like a spinlock,
// but prevents excessively long spinning.

struct pthread_spinlock_internal_t
{
    struct Lock lock;
};

static_assert(sizeof(pthread_spinlock_t) == sizeof(struct pthread_spinlock_internal_t),
              "pthread_spinlock_t should actually be pthread_spinlock_internal_t.");

static_assert(alignof(pthread_spinlock_t) >= 4,
              "pthread_spinlock_t should fulfill the alignment of pthread_spinlock_internal_t.");

static inline __always_inline
struct pthread_spinlock_internal_t* __get_internal_spinlock(pthread_spinlock_t* lock)
{
    return (struct pthread_spinlock_internal_t*)lock;
}

int pthread_spin_init(pthread_spinlock_t* lock_interface, int pshared)
{
    struct pthread_spinlock_internal_t* lock = __get_internal_spinlock(lock_interface);
    Lock_init(&lock->lock, pshared);
    return 0;
}

int pthread_spin_destroy(pthread_spinlock_t* lock_interface)
{
    struct pthread_spinlock_internal_t* lock = __get_internal_spinlock(lock_interface);
    return Lock_trylock(&lock->lock) ? 0 : EBUSY;
}

int pthread_spin_trylock(pthread_spinlock_t* lock_interface)
{
    struct pthread_spinlock_internal_t* lock = __get_internal_spinlock(lock_interface);
    return Lock_trylock(&lock->lock) ? 0 : EBUSY;
}

int pthread_spin_lock(pthread_spinlock_t* lock_interface)
{
    struct pthread_spinlock_internal_t* lock = __get_internal_spinlock(lock_interface);
    for (int i = 0; i < 10000; ++i) {
        if (Lock_trylock(&lock->lock)) {
            return 0;
        }
    }
    Lock_lock(&lock->lock);
    return 0;
}

int pthread_spin_unlock(pthread_spinlock_t* lock_interface)
{
    struct pthread_spinlock_internal_t* lock = __get_internal_spinlock(lock_interface);
    Lock_unlock(&lock->lock);
    return 0;
}

__END_DECLS
