#define __USE_BSD

#include "stdio_extra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define MINIMUM(a, b) (((a) < (b)) ? (a) : (b))

struct state
{
    char* string;  /* actual stream */
    char** pbuf;   /* point to the stream */
    size_t* psize; /* point to min(pos, len) */
    size_t pos;    /* current position */
    size_t size;   /* number of allocated char */
    size_t len;    /* length of the data */
};

/*
 * This is sqrt(SIZE_MAX+1), as s1*s2 <= SIZE_MAX
 * if both s1 < MUL_NO_OVERFLOW and s2 < MUL_NO_OVERFLOW
 */
#define MUL_NO_OVERFLOW ((size_t)1 << (sizeof(size_t) * 4))

#define explicit_bzero(p, s) memset(p, 0, s)

static void* recallocarray(void* ptr, size_t oldnmemb, size_t newnmemb, size_t size)
{
    size_t oldsize, newsize;
    void* newptr;

    if (ptr == NULL)
        return calloc(newnmemb, size);

    if ((newnmemb >= MUL_NO_OVERFLOW || size >= MUL_NO_OVERFLOW) && newnmemb > 0
        && SIZE_MAX / newnmemb < size) {
        errno = ENOMEM;
        return NULL;
    }
    newsize = newnmemb * size;

    if ((oldnmemb >= MUL_NO_OVERFLOW || size >= MUL_NO_OVERFLOW) && oldnmemb > 0
        && SIZE_MAX / oldnmemb < size) {
        errno = EINVAL;
        return NULL;
    }
    oldsize = oldnmemb * size;

    /*
     * Don't bother too much if we're shrinking just a bit,
     * we do not shrink for series of small steps, oh well.
     */
    if (newsize <= oldsize) {
        size_t d = oldsize - newsize;

        if (d < oldsize / 2 && d < getpagesize()) {
            memset((char*)ptr + newsize, 0, d);
            return ptr;
        }
    }

    newptr = malloc(newsize);
    if (newptr == NULL)
        return NULL;

    if (newsize > oldsize) {
        memcpy(newptr, ptr, oldsize);
        memset((char*)newptr + oldsize, 0, newsize - oldsize);
    }
    else
        memcpy(newptr, ptr, newsize);

    explicit_bzero(ptr, oldsize);
    free(ptr);

    return newptr;
}

static int memstream_write(void* v, const char* b, int l)
{
    struct state* st = (struct state*)v;
    char* p;
    size_t i, end;

    end = (st->pos + l);

    if (end >= st->size) {
        /* 1.6 is (very) close to the golden ratio. */
        size_t sz = st->size * 8 / 5;

        if (sz < end + 1)
            sz = end + 1;
        p = (char*)recallocarray(st->string, st->size, sz, 1);
        if (!p)
            return (-1);
        bzero(p + st->size, sz - st->size);
        *st->pbuf = st->string = p;
        st->size = sz;
    }

    for (i = 0; i < l; i++)
        st->string[st->pos + i] = b[i];
    st->pos += l;

    if (st->pos > st->len) {
        st->len = st->pos;
        st->string[st->len] = '\0';
    }

    *st->psize = st->pos;

    return (i);
}

static fpos_t memstream_seek(void* v, fpos_t off, int whence)
{
    struct state* st = (struct state*)v;
    size_t base = 0;

    switch (whence) {
        case SEEK_SET:
            break;
        case SEEK_CUR:
            base = st->pos;
            break;
        case SEEK_END:
            base = st->len;
            break;
    }

    if ((off > 0 && off > SIZE_MAX - base) || (off < 0 && base < -off)) {
        errno = EOVERFLOW;
        return (-1);
    }

    st->pos = base + off;
    *st->psize = MINIMUM(st->pos, st->len);

    return (st->pos);
}

static int memstream_close(void* v)
{
    struct state* st = (struct state*)v;

    free(st);

    return (0);
}

FILE* open_memstream(char** pbuf, size_t* psize)
{
    struct state* st;
    FILE* fp;

    if (pbuf == NULL || psize == NULL) {
        errno = EINVAL;
        return (NULL);
    }

    if ((st = (struct state*)malloc(sizeof(*st))) == NULL)
        return (NULL);

    fp = funopen(st, NULL, memstream_write, memstream_seek, memstream_close);
    if (/* (fp = __sfp())*/ fp == NULL) {
        free(st);
        return (NULL);
    }

    st->size = BUFSIZ;
    if ((st->string = (char*)calloc(1, st->size)) == NULL) {
        free(st);
        fp->_flags = 0;
        return (NULL);
    }

    *st->string = '\0';
    st->pos = 0;
    st->len = 0;
    st->pbuf = pbuf;
    st->psize = psize;

    *pbuf = st->string;
    *psize = st->len;

    //	fp->_flags = __SWR;
    //	fp->_file = -1;
    //	fp->_cookie = st;
    //	fp->_read = NULL;
    //	fp->_write = memstream_write;
    //	fp->_seek = memstream_seek;
    //	fp->_close = memstream_close;
    _SET_ORIENTATION(fp, -1);

    return (fp);
}
