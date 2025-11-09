#include "stdio_extra.h"
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <stdio_ext.h>

#define CHECK_FP(fp)                                                                               \
    if (fp == NULL)                                                                                \
    __fortify_fatal("%s: null FILE*", __FUNCTION__)

#define FLOCKFILE(fp)                                                                              \
    if (__fsetlocking(fp, FSETLOCKING_QUERY) == FSETLOCKING_INTERNAL)                              \
    flockfile(fp)
#define FUNLOCKFILE(fp)                                                                            \
    if (__fsetlocking(fp, FSETLOCKING_QUERY) == FSETLOCKING_INTERNAL)                              \
    funlockfile(fp)

#define __sferror(p) (((p)->_flags & __SERR) != 0)
#define __sclearerr(p) ((void)((p)->_flags &= ~(__SERR | __SEOF)))

class ScopedFileLock
{
public:
    explicit ScopedFileLock(FILE* fp)
        : fp_(fp)
    {
        FLOCKFILE(fp_);
    }
    ~ScopedFileLock()
    {
        FUNLOCKFILE(fp_);
    }

private:
    FILE* fp_;
};

__BEGIN_DECLS

#undef stdin
#undef stdout
#undef stderr
FILE* stdin = &(__sF[0]);
FILE* stdout = &(__sF[1]);
FILE* stderr = &(__sF[2]);

static __noreturn __printflike(1, 2) void __fortify_fatal(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    async_safe_fatal_va_list("FORTIFY", fmt, args);
    va_end(args);
    abort();
}

#define _PATH_TTY "/dev/tty"

char* ctermid(char* s)
{
    return s ? strcpy(s, _PATH_TTY) : const_cast<char*>(_PATH_TTY);
}

int fileno_unlocked(FILE* fp)
{
    CHECK_FP(fp);
    int fd = fp->_file;
    if (fd == -1) {
        errno = EBADF;
        return -1;
    }
    return fd;
}

/*
int fileno(FILE* fp) {
  CHECK_FP(fp);
  ScopedFileLock sfl(fp);
  return fileno_unlocked(fp);
}
*/

void clearerr_unlocked(FILE* fp)
{
    CHECK_FP(fp);
    return __sclearerr(fp);
}

int feof_unlocked(FILE* fp)
{
    CHECK_FP(fp);
    return ((fp->_flags & __SEOF) != 0);
}

/*
int feof(FILE* fp) {
  CHECK_FP(fp);
  ScopedFileLock sfl(fp);
  return feof_unlocked(fp);
}
*/

int ferror_unlocked(FILE* fp)
{
    CHECK_FP(fp);
    return __sferror(fp);
}

__END_DECLS
