#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_spinlock_t spinlock;

void* run(void* arg)
{
    int volatile* p = (int*)arg;

    while ((*p) < 1000) {
        pthread_spin_lock(&spinlock);

        (*p)++;
        printf("pthread_self %lu, count = %d\n", pthread_self(), *p);

        pthread_spin_unlock(&spinlock);
        usleep(500);
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{

    pthread_t tid[10] = {0};
    pthread_spin_init(&spinlock, PTHREAD_PROCESS_SHARED);
    int i, j;
    int count = 0;
    int num = 8;
    for (i = 0; i < num; ++i) {
        pthread_create(&tid[i], NULL, run, (void*)&count);
    }

    for (j = 0; j < num; ++j) {
        pthread_join(tid[j], NULL);
    }
    pthread_spin_destroy(&spinlock);
    return 0;
}