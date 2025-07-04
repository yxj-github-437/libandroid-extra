/*
 * Copyright (c) 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Guido van Rossum.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)glob.h	8.1 (Berkeley) 6/2/93
 * $FreeBSD$
 */

#ifndef _GLOB_H_
#define _GLOB_H_

#include <sys/cdefs.h>
#include <sys/types.h>

struct dirent;
struct stat;

typedef struct {
  size_t gl_pathc;	/* Count of total paths so far. */
  size_t gl_matchc;	/* Count of paths matching pattern. */
  size_t gl_offs;		/* Reserved at beginning of gl_pathv. */
  int gl_flags;		/* Copy of flags parameter to glob. */

  /** List of paths matching pattern. */
  char* _Nullable * _Nullable gl_pathv;

  /** Copy of `__error_callback` parameter to glob. */
  int (* _Nullable gl_errfunc)(const char* _Nonnull __failure_path, int __failure_errno);

  /** Called instead of closedir() when GLOB_ALTDIRFUNC flag is specified. */
  void (* _Nullable gl_closedir)(void* _Nonnull);
  /** Called instead of readdir() when GLOB_ALTDIRFUNC flag is specified. */
  struct dirent* _Nullable (* _Nonnull gl_readdir)(void* _Nonnull);
  /** Called instead of opendir() when GLOB_ALTDIRFUNC flag is specified. */
  void* _Nullable (* _Nonnull gl_opendir)(const char* _Nonnull);
  /** Called instead of lstat() when GLOB_ALTDIRFUNC flag is specified. */
  int (* _Nullable gl_lstat)(const char* _Nonnull, struct stat* _Nonnull);
  /** Called instead of stat() when GLOB_ALTDIRFUNC flag is specified. */
  int (* _Nullable gl_stat)(const char* _Nonnull, struct stat* _Nonnull);
} glob_t;

/* Believed to have been introduced in 1003.2-1992 */
#define GLOB_APPEND	0x0001	/* Append to output from previous call. */
#define GLOB_DOOFFS	0x0002	/* Prepend `gl_offs` null pointers (leaving space for exec, say). */
#define GLOB_ERR	0x0004	/* Return on error. */
#define GLOB_MARK	0x0008	/* Append "/" to the names of returned directories. */
#define GLOB_NOCHECK	0x0010	/* Return pattern itself if nothing matches. */
#define GLOB_NOSORT	0x0020	/* Don't sort. */
#define GLOB_NOESCAPE	0x2000	/* Disable backslash escaping. */

/* Error values returned by glob(3) */
#define GLOB_NOSPACE	(-1)	/* Malloc call failed. */
#define GLOB_ABORTED	(-2)	/* Unignored error. */
#define GLOB_NOMATCH	(-3)	/* No match and GLOB_NOCHECK was not set. */

#if __USE_BSD
#define GLOB_ALTDIRFUNC	0x0040	/* Use alternately specified directory funcs. */
#define GLOB_BRACE	0x0080	/* Expand braces like csh. */
#define GLOB_MAGCHAR	0x0100	/* Set in `gl_flags` if the pattern had globbing characters. */
#define GLOB_NOMAGIC	0x0200	/* GLOB_NOCHECK without magic chars (csh). */
#define GLOB_QUOTE	0x0400	/* Quote special chars with \. */
#define GLOB_TILDE	0x0800	/* Expand tilde names from the passwd file. */
#define GLOB_LIMIT	0x1000	/* limit number of returned paths */
#endif

__BEGIN_DECLS


#if __BIONIC_AVAILABILITY_GUARD(28) || __ANDROID_EXTRA
int glob(const char* _Nonnull __pattern, int __flags, int (* _Nullable __error_callback)(const char* _Nonnull __failure_path, int __failure_errno), glob_t* _Nonnull __result_ptr) __INTRODUCED_IN(28);
void globfree(glob_t* _Nonnull __result_ptr) __INTRODUCED_IN(28);
#endif /* __BIONIC_AVAILABILITY_GUARD(28) */


__END_DECLS

#endif
