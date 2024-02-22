#include "bionic_futex.h"
#include <assert.h>   // assert_static
#include <errno.h>
#include <pthread.h>
#include <stdalign.h> // alignof
#include <stdatomic.h>
#include <stdint.h>

__BEGIN_DECLS

int pthread_barrierattr_init(pthread_barrierattr_t* attr)
{
    *attr = 0;
    return 0;
}

int pthread_barrierattr_destroy(pthread_barrierattr_t* attr)
{
    *attr = 0;
    return 0;
}

int pthread_barrierattr_getpshared(const pthread_barrierattr_t* attr, int* pshared)
{
    *pshared = (*attr & 1) ? PTHREAD_PROCESS_SHARED : PTHREAD_PROCESS_PRIVATE;
    return 0;
}

int pthread_barrierattr_setpshared(pthread_barrierattr_t* attr, int pshared)
{
    if (pshared == PTHREAD_PROCESS_SHARED) {
        *attr |= 1;
    }
    else {
        *attr &= ~1;
    }
    return 0;
}


enum BarrierState
{
    WAIT,
    RELEASE,
};

struct pthread_barrier_internal_t
{
    // One barrier can be used for unlimited number of cycles. In each cycle, [init_count]
    // threads must call pthread_barrier_wait() before any of them successfully return from
    // the call. It is undefined behavior if there are more than [init_count] threads call
    // pthread_barrier_wait() in one cycle.
    uint32_t init_count;
    // Barrier state. It is WAIT if waiting for more threads to enter the barrier in this cycle,
    // otherwise threads are leaving the barrier.
    _Atomic(enum BarrierState) state;
    // Number of threads having entered but not left the barrier in this cycle.
    atomic_uint wait_count;
    // Whether the barrier is shared across processes.
    bool pshared;
    uint32_t __reserved[4];
};

static_assert(
    sizeof(pthread_barrier_t) == sizeof(struct pthread_barrier_internal_t),
    "pthread_barrier_t should actually be struct pthread_barrier_internal_t in implementation.");

static_assert(
    alignof(pthread_barrier_t) >= 4,
    "pthread_barrier_t should fulfill the alignment of struct pthread_barrier_internal_t.");

static inline struct pthread_barrier_internal_t* __get_internal_barrier(pthread_barrier_t* barrier)
{
    return (struct pthread_barrier_internal_t*)barrier;
}

int pthread_barrier_init(pthread_barrier_t* barrier_interface, const pthread_barrierattr_t* attr,
                         unsigned count)
{
    struct pthread_barrier_internal_t* barrier = __get_internal_barrier(barrier_interface);
    if (count == 0) {
        return EINVAL;
    }
    barrier->init_count = count;
    atomic_init(&barrier->state, WAIT);
    atomic_init(&barrier->wait_count, 0);
    barrier->pshared = false;
    if (attr != NULL && (*attr & 1)) {
        barrier->pshared = true;
    }
    return 0;
}


// According to POSIX standard, pthread_barrier_wait() synchronizes memory between participating
// threads. It means all memory operations made by participating threads before calling
// pthread_barrier_wait() can be seen by all participating threads after the function call.
// We establish this by making a happens-before relation between all threads entering the barrier
// with the last thread entering the barrier, and a happens-before relation between the last
// thread entering the barrier with all threads leaving the barrier.
int pthread_barrier_wait(pthread_barrier_t* barrier_interface)
{
    struct pthread_barrier_internal_t* barrier = __get_internal_barrier(barrier_interface);

    // Wait until all threads for the previous cycle have left the barrier. This is needed
    // as a participating thread can call pthread_barrier_wait() again before other
    // threads have left the barrier. Use acquire operation here to synchronize with
    // the last thread leaving the previous cycle, so we can read correct wait_count below.
    while (atomic_load_explicit(&barrier->state, memory_order_acquire) == RELEASE) {
        __futex_wait_ex_with_timeout(&barrier->state, barrier->pshared, RELEASE, false, NULL);
    }

    uint32_t prev_wait_count = atomic_load_explicit(&barrier->wait_count, memory_order_relaxed);
    while (true) {
        // It happens when there are more than [init_count] threads trying to enter the barrier
        // at one cycle. We read the POSIX standard as disallowing this, since additional arriving
        // threads are not synchronized with respect to the barrier reset. We also don't know of
        // any reasonable cases in which this would be intentional.
        if (prev_wait_count >= barrier->init_count) {
            return EINVAL;
        }
        // Use memory_order_acq_rel operation here to synchronize between all threads entering
        // the barrier with the last thread entering the barrier.
        if (atomic_compare_exchange_weak_explicit(&barrier->wait_count,
                                                  &prev_wait_count,
                                                  prev_wait_count + 1u,
                                                  memory_order_acq_rel,
                                                  memory_order_relaxed)) {
            break;
        }
    }

    int result = 0;
    if (prev_wait_count + 1 == barrier->init_count) {
        result = PTHREAD_BARRIER_SERIAL_THREAD;
        if (prev_wait_count != 0) {
            // Use release operation here to synchronize between the last thread entering the
            // barrier with all threads leaving the barrier.
            atomic_store_explicit(&barrier->state, RELEASE, memory_order_release);
            __futex_wake_ex(&barrier->state, barrier->pshared, prev_wait_count);
        }
    }
    else {
        // Use acquire operation here to synchronize between the last thread entering the
        // barrier with all threads leaving the barrier.
        while (atomic_load_explicit(&barrier->state, memory_order_acquire) == WAIT) {
            __futex_wait_ex_with_timeout(&barrier->state, barrier->pshared, WAIT, false, NULL);
        }
    }
    // Use release operation here to make it not reordered with previous operations.
    if (atomic_fetch_sub_explicit(&barrier->wait_count, 1, memory_order_release) == 1) {
        // Use release operation here to synchronize with threads entering the barrier for
        // the next cycle, or the thread calling pthread_barrier_destroy().
        atomic_store_explicit(&barrier->state, WAIT, memory_order_release);
        __futex_wake_ex(&barrier->state, barrier->pshared, barrier->init_count);
    }
    return result;
}

int pthread_barrier_destroy(pthread_barrier_t* barrier_interface)
{
    struct pthread_barrier_internal_t* barrier = __get_internal_barrier(barrier_interface);
    if (barrier->init_count == 0) {
        return EINVAL;
    }
    // Use acquire operation here to synchronize with the last thread leaving the barrier.
    // So we can read correct wait_count below.
    while (atomic_load_explicit(&barrier->state, memory_order_acquire) == RELEASE) {
        __futex_wait_ex_with_timeout(&barrier->state, barrier->pshared, RELEASE, false, NULL);
    }
    if (atomic_load_explicit(&barrier->wait_count, memory_order_relaxed) != 0) {
        return EBUSY;
    }
    barrier->init_count = 0;
    return 0;
}

__END_DECLS
