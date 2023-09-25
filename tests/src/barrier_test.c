#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define USE_BARRIER 1 /* 是否使用屏障，1使用，0不使用 */

/* 屏障 */
#if USE_BARRIER
static pthread_barrier_t barrier;
#endif

void* thread0_entry(void* data)
{
    int temp = 0;
    uint32_t count = 0;

    printf("thread0 startup\r\n");
#if USE_BARRIER
    temp = pthread_barrier_wait(&barrier);
    printf("thread0 barrier return[%d]\r\n", temp);
#endif
    for (; count < 5;) {
        printf("thread0 count [%d]\r\n", count++);
        sleep(1); /* 主动让出cpu */
    }
    
    return NULL;
}

void* thread1_entry(void* data)
{
    int temp = 0;
    uint32_t count = 0;

    sleep(1); /* 让线程0先执行 */
    printf("thread1 startup\r\n");
#if USE_BARRIER
    temp = pthread_barrier_wait(&barrier);
    printf("thread1 barrier return[%d]\r\n", temp);
#endif
    for (; count < 10;) {
        printf("thread1 count [%d]\r\n", count++);
        sleep(1); /* 主动让出cpu */
    }
    
    return NULL;
}

void* thread2_entry(void* data)
{
    int temp = 0;
    uint32_t count = 0;

    sleep(2); /* 让线程1先执行 */
    printf("thread2 startup\r\n");
#if USE_BARRIER
    temp = pthread_barrier_wait(&barrier);
    printf("thread2 barrier return[%d]\r\n", temp);
#endif
    for (; count < 10;) {
        printf("thread2 count [%d]\r\n", count++);
        sleep(1); /* 主动让出cpu */
    }
    
    return NULL;
}

int main(int argc, char** argv)
{
    pthread_t thread0, thread1, thread2;
    void* retval;

#if USE_BARRIER
    pthread_barrier_init(&barrier, NULL, 3);
#endif
    pthread_create(&thread0, NULL, thread0_entry, NULL);
    pthread_create(&thread1, NULL, thread1_entry, NULL);
    pthread_create(&thread2, NULL, thread2_entry, NULL);
    pthread_join(thread0, &retval);
    pthread_join(thread1, &retval);
    pthread_join(thread2, &retval);

#if USE_BARRIER
    pthread_barrier_destroy(&barrier);
#endif

    return 0;
}