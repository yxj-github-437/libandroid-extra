#define __USE_BSD

#include "stdio_extra.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>


__BEGIN_DECLS

/*
 * Return the (stdio) flags for a given mode.  Store the flags
 * to be passed to an open() syscall through *optr.
 * Return 0 on error.
 */
static int __sflags(const char* mode, int* optr)
{
    int ret, m, o;

    switch (*mode++) {

        case 'r': /* open for reading */
            ret = __SRD;
            m = O_RDONLY;
            o = 0;
            break;

        case 'w': /* open for writing */
            ret = __SWR;
            m = O_WRONLY;
            o = O_CREAT | O_TRUNC;
            break;

        case 'a': /* open for appending */
            ret = __SWR;
            m = O_WRONLY;
            o = O_CREAT | O_APPEND;
            break;

        default: /* illegal mode */
            errno = EINVAL;
            return (0);
    }

    while (*mode != '\0')
        switch (*mode++) {
            case 'b':
                break;
            case '+':
                ret = __SRW;
                m = O_RDWR;
                break;
            case 'e':
                o |= O_CLOEXEC;
                break;
            case 'x':
                if (o & O_CREAT)
                    o |= O_EXCL;
                break;
            default:
                /*
                 * Lots of software passes other extension mode
                 * letters, like Window's 't'
                 */
#if 0
			errno = EINVAL;
			return (0);
#else
                break;
#endif
        }

    *optr = m | o;
    return (ret);
}

typedef struct fmemopen_cookie
{
    char* buf;
    char* allocation;
    size_t capacity;
    size_t size;
    size_t offset;
    bool append;
} fmemopen_cookie;

static int fmemopen_read(void* cookie, char* buf, int n)
{
    fmemopen_cookie* ck = (fmemopen_cookie*)cookie;

    if ((size_t)n > ck->size - ck->offset)
        n = ck->size - ck->offset;

    if (n > 0) {
        memmove(buf, ck->buf + ck->offset, n);
        ck->offset += n;
    }
    return n;
}

static int fmemopen_write(void* cookie, const char* buf, int n)
{
    fmemopen_cookie* ck = (fmemopen_cookie*)cookie;

    // We don't need to add the trailing NUL if there's already a trailing NUL
    // in the data we're writing.
    size_t space_for_null = (n > 0 && buf[n - 1] != '\0') ? 1 : 0;

    // Undo any seeking/reading on an append-only stream.
    if (ck->append)
        ck->offset = ck->size;

    // How much can we actually fit?
    if ((size_t)n + space_for_null > ck->capacity - ck->offset) {
        n = ck->capacity - ck->offset - space_for_null;
        // Give up if we don't even have room for one byte of userdata.
        if (n <= 0) {
            errno = ENOSPC;
            return -1;
        }
    }

    if (n > 0) {
        memmove(ck->buf + ck->offset, buf, n);
        ck->offset += n;
        // Is this the furthest we've ever been?
        if (ck->offset >= ck->size) {
            if (buf[n - 1] != '\0')
                ck->buf[ck->offset] = '\0';
            ck->size = ck->offset;
        }
    }
    return n;
}

static fpos_t fmemopen_seek(void* cookie, fpos_t offset, int whence)
{
    fmemopen_cookie* ck = (fmemopen_cookie*)cookie;

    if (whence == SEEK_SET && (offset >= 0 && (size_t)offset <= ck->capacity)) {
        return (ck->offset = offset);
    }
    else if (whence == SEEK_CUR && (ck->offset + offset <= ck->capacity)) {
        return (ck->offset += offset);
    }
    else if (whence == SEEK_END && (offset <= 0 && (size_t)(-offset) <= ck->size)) {
        return (ck->offset = ck->size + offset);
    }
    errno = EINVAL;
    return -1;
}

static int fmemopen_close(void* cookie)
{
    fmemopen_cookie* ck = (fmemopen_cookie*)cookie;
    free(ck->allocation);
    free(ck);
    return 0;
}

FILE* fmemopen(void* buf, size_t capacity, const char* mode)
{
    int flags;
    if (__sflags(mode, &flags) == 0) {
        errno = EINVAL;
        return NULL;
    }

    fmemopen_cookie* ck = (fmemopen_cookie*)calloc(sizeof(fmemopen_cookie), 1);
    if (ck == NULL)
        return NULL;

    ck->buf = (char*)buf;
    ck->capacity = capacity;

    if (ck->buf == NULL)
        ck->buf = ck->allocation = (char*)calloc(capacity, 1);
    if (ck->buf == NULL) {
        free(ck);
        return NULL;
    }

    FILE* fp = funopen(ck,
                       (flags & O_WRONLY) ? NULL : fmemopen_read,
                       (flags & O_RDONLY) ? NULL : fmemopen_write,
                       fmemopen_seek,
                       fmemopen_close);
    if (fp == NULL) {
        fmemopen_close(ck);
        return NULL;
    }

    if (mode[0] == 'a') {
        ck->size = strnlen(ck->buf, ck->capacity);
        ck->offset = ck->size;
        ck->append = true;
    }
    else if (mode[0] == 'r') {
        ck->size = capacity;
        ck->offset = 0;
    }
    else if (mode[0] == 'w') {
        ck->size = 0;
        ck->offset = 0;
        if (capacity > 0) {
            ck->buf[0] = '\0';
        }
    }

    return fp;
}

__END_DECLS
