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

#if 0

static int __sflush(FILE* fp) {
  // Flushing a read-only file is a no-op.
  if ((fp->_flags & __SWR) == 0) return 0;

  // Flushing a file without a buffer is a no-op.
  unsigned char* p = fp->_bf._base;
  if (p == nullptr) return 0;

  // Set these immediately to avoid problems with longjmp and to allow
  // exchange buffering (via setvbuf) in user write function.
  int n = fp->_p - p;
  fp->_p = p;
  fp->_w = (fp->_flags & (__SLBF|__SNBF)) ? 0 : fp->_bf._size;

  while (n > 0) {
    int written = (*fp->_write)(fp->_cookie, reinterpret_cast<char*>(p), n);
    if (written <= 0) {
      fp->_flags |= __SERR;
      return EOF;
    }
    n -= written, p += written;
  }
  return 0;
}

static struct glue __sglue = { nullptr, 3, __sF };
// static struct glue* lastglue = &__sglue;

static int _fwalk(int (*callback)(FILE*)) {
  int result = 0;
  for (glue* g = &__sglue; g != nullptr; g = g->next) {
    FILE* fp = g->iobs;
    for (int n = g->niobs; --n >= 0; ++fp) {
      if (fp->_flags != 0 && (fp->_flags & __SIGN) == 0) {
        result |= (*callback)(fp);
      }
    }
  }
  return result;
}

static int __sflush_locked(FILE* fp) {
  ScopedFileLock sfl(fp);
  return __sflush(fp);
}

static int fflush_all() {
  return _fwalk(__sflush_locked);
}

int fflush_unlocked(FILE* fp) {
  if (fp == nullptr) return fflush_all();
  if ((fp->_flags & (__SWR | __SRW)) == 0) {
    errno = EBADF;
    return EOF;
  }
  return __sflush(fp);
}


#endif

__END_DECLS
