#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>

int main()
{
    int fd[2];
    int p[2];

    char OutPipe[100], InPipe[100];
    pipe(fd);								/* 创建一个管道 */

    while ((p[0] = fork()) == -1);
    if (p[0] == 0)
    {
        printf("p[0]\n");
        lockf(fd[1], 1, 0);					/* 加锁 */
        sprintf(OutPipe, "Child 1 is sending a message!");			/* 把串放入数组outpipe中 */
        write(fd[1], OutPipe, 50);			/* 向管道写长为50字节的串 */
        sleep(1);							/* 自我阻塞1秒 */
        lockf(fd[1], 0, 0);					/* 解锁 */
        exit(0);
    }
    else
    {
        while ((p[1] = fork()) == -1);
        if (p[1] == 0)
        {
            printf("p[1]\n");
            lockf(fd[1], 1, 0);				/* 加锁，互斥 */
            sprintf(OutPipe, "Child 2 is sending a message!");		/* 把串放入数组outpipe中 */
            write(fd[1], OutPipe, 50);		/* 向管道写长为50字节的串 */
            sleep(1);						/* 自我阻塞1秒 */
            lockf(fd[1], 0, 0);				/* 解锁 */
            exit(0);
        }
        else
        {
            printf("parent\n");
            wait(0);						/* 同步 */
            read(fd[0], InPipe, 50);
            printf("%s\n", InPipe);			/* 从管道中读长为50字节的串 */
            wait(0);
            read(fd[0], InPipe, 50);
            printf("%s\n", InPipe);
            exit(0);
        }
    }
}
