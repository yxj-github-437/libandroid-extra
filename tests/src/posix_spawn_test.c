#include <errno.h>
#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

extern char** environ;

int main(int argc, char* argv[])
{
    posix_spawnattr_t attr;
    posix_spawn_file_actions_t fact;
    pid_t pid;
    char cmd[] = "echo";
    char opt[] = "hello world";
    char* args[3];
    args[0] = cmd;
    args[1] = opt;
    args[2] = NULL;
    posix_spawnattr_init(&attr);
    posix_spawn_file_actions_init(&fact);
    if (posix_spawn(&pid, "/system/bin/echo", &fact, &attr, args, environ) != 0)
    {
        perror("posix_spawn");
        exit(-1);
    }

    printf("pid=%d,child pid = %d\n", getpid(), pid);
    int stat = 0;
    waitpid(pid, &stat, 0);
    printf("stat is %d\n", stat);
    return 0;
}
