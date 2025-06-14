/*
 * Copyright (C) 2016 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#pragma once

#include <sys/cdefs.h>

 __BEGIN_DECLS
#if defined(__LP64__)
struct __sbuf {
	unsigned char* _base;
	size_t _size;
};
#else
struct __sbuf {
	unsigned char *_base;
	int     _size;
};
#endif
#if defined(__LP64__)
typedef int64_t _struct_file_off_t;
#else
typedef __kernel_off_t _struct_file_off_t;
#endif


 /** The opaque structure implementing `FILE`. Do not make any assumptions about its content. */
 struct __sFILE {
	unsigned char *_p;      /* current position in (some) buffer */
	int     _r;             /* read space left for getc() */
	int     _w;             /* write space left for putc() */
 #if defined(__LP64__)
	int     _flags;         /* flags, below; this FILE is free if 0 */
	int     _file;          /* fileno, if Unix descriptor, else -1 */
 #else
	short   _flags;         /* flags, below; this FILE is free if 0 */
	short   _file;          /* fileno, if Unix descriptor, else -1 */
 #endif
	struct  __sbuf _bf;     /* the buffer (at least 1 byte, if !NULL) */
    int     _lbfsize;       /* 0 or -_bf._size, for inline putc */

    /* operations */
    void    *_cookie;       /* cookie passed to io functions */
	int     (*_close)(void *);
	int     (*_read)(void *, char *, int);
	_struct_file_off_t      (*_seek)(void *, _struct_file_off_t, int);
	int     (*_write)(void *, const char *, int);

	/* extension data, to avoid further ABI breakage */
	struct  __sbuf _ext;
	/* data for long sequences of ungetc() */
	unsigned char *_up;     /* saved _p when _p is doing ungetc data */
	int     _ur;            /* saved _r when _r is counting ungetc data */

	/* tricks to meet minimum requirements even when malloc() fails */
	unsigned char _ubuf[3]; /* guarantee an ungetc() buffer */
	unsigned char _nbuf[1]; /* guarantee a getc() buffer */

	/* separate buffer for fgetln() when line crosses buffer boundary */
	struct  __sbuf _lb;     /* buffer for fgetln() */

	/* Unix stdio files get aligned to block boundaries on fseek() */
	int     _blksize;       /* stat.st_blksize (may be != _bf._size) */
	_struct_file_off_t      _offset;        /* current lseek offset */
};
#define __SLBF  0x0001          /* line buffered */
#define __SNBF  0x0002          /* unbuffered */
#define __SRD   0x0004          /* OK to read */
#define __SWR   0x0008          /* OK to write */
	/* RD and WR are never simultaneously asserted */
#define __SRW   0x0010          /* open for reading & writing */
#define __SEOF  0x0020          /* found EOF */
#define __SERR  0x0040          /* found error */
#define __SMBF  0x0080          /* _buf is from malloc */
#define __SAPP  0x0100          /* fdopen()ed in append mode */
#define __SSTR  0x0200          /* this is an sprintf/snprintf string */
#define __SOPT  0x0400          /* do fseek() optimization */
#define __SNPT  0x0800          /* do not do fseek() optimization */
#define __SOFF  0x1000          /* set iff _offset is in fact correct */
#define __SMOD  0x2000          /* true => fgetln modified _p text */
#define __SALC  0x4000          /* allocate string space dynamically */
#define __SIGN  0x8000          /* ignore this file in _fwalk */

 __END_DECLS
