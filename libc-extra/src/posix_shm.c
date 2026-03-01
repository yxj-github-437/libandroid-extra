#include <errno.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <string.h>
#include <sys/mman.h>

#include "tmpdir_get.h"

int shm_open(const char* name, int oflag, mode_t mode)
{
    while (name[0] == '/')
        name++;
    if (name[0] == '\0') {
        errno = EINVAL;
        return -1;
    }

    char fname[PATH_MAX];
    tmpdir_get(fname, PATH_MAX);
    size_t len = strlen(fname);
    snprintf(fname + len, PATH_MAX - len, "/%s", name);
    return open(fname, oflag | O_CLOEXEC, mode);
}

int shm_unlink(const char* name)
{
    while (name[0] == '/')
        name++;
    if (name[0] == '\0') {
        errno = EINVAL;
        return -1;
    }

    char fname[PATH_MAX];
    tmpdir_get(fname, PATH_MAX);
    size_t len = strlen(fname);
    snprintf(fname + len, PATH_MAX - len, "/%s", name);
    return unlink(fname);
}
