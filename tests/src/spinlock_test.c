#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <unistd.h>

pthread_spinlock_t spinLock;
int lockCnt = 10;

void printSelfThreadInfo(const char* s, int cnt);
void* threadEntryOne(void* arg);
void* threadEntryTwo(void* arg);

#define SYS_SUCC 0
#define SYS_ERR -1

pthread_t gThreadId1, gThreadId2;

int main(int argc, char** argv)
{
    unsigned int i = 0;
    int sysRet = -1;

    /*1,initialize spin lock*/
    pthread_spin_init(&spinLock, PTHREAD_PROCESS_PRIVATE);

    printf("process pid = %d\n", getpid());

    sysRet = pthread_create(&gThreadId1, NULL, threadEntryOne, NULL);
    if (sysRet == SYS_ERR) {
        printf("can't create threadOne...\n");
        exit(1);
    }

    sysRet = pthread_create(&gThreadId2, NULL, threadEntryTwo, NULL);
    if (sysRet == SYS_ERR) {
        printf("can't create threadTwo...\n");
        exit(1);
    }

    pthread_join(gThreadId1, NULL);
    pthread_join(gThreadId2, NULL);

    pthread_spin_destroy(&spinLock);
    return 0;
}

void printSelfThreadInfo(const char* s, int cnt)
{
    pid_t tid;

    tid = pthread_self();
    printf("%s:tid = %u,lockCnt=%d\n", s, tid, cnt);
}

void* threadEntryOne(void* arg)
{
    char threadName[16];
    prctl(PR_SET_NAME, "threadEntryOne");
    prctl(PR_GET_NAME, threadName);
    for (;;) {
        pthread_spin_lock(&spinLock);
        // pthread_spin_lock(&spinLock);

        printSelfThreadInfo(threadName, lockCnt);

        if (lockCnt <= 0)
            break;

        lockCnt--;

        sleep(1);
        pthread_spin_unlock(&spinLock);
        // sleep(1);
    }

    pthread_spin_unlock(&spinLock);
    return NULL;
}

void* threadEntryTwo(void* arg)
{
    char threadName[16];
    prctl(PR_SET_NAME, "threadEntryTwo");// 设置线程名
    prctl(PR_GET_NAME, threadName);      // 获取线程名

    for (;;) {
        pthread_spin_lock(&spinLock);
        // pthread_spin_lock(&spinLock);spin锁不支持递归调用,重复调用也会自旋
        printSelfThreadInfo(threadName, lockCnt);

        if (lockCnt <= 0)
            break;

        lockCnt--;

        sleep(1);
        pthread_spin_unlock(&spinLock);// 实测:放锁后立刻又获取锁通常可以成功
                                       // sleep(1);
    }

    pthread_spin_unlock(&spinLock);// 循环结束后需要记得放锁,否则其它线程永远要自旋
    return NULL;
}