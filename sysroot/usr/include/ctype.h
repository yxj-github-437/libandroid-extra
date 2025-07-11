/*
 * Copyright (C) 2014 The Android Open Source Project
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

/**
 * @file ctype.h
 * @brief ASCII character classification.
 */

#include <sys/cdefs.h>
#include <xlocale.h>

/* All the functions in this file are trivial, being but a single
 * instruction on most architectures. For that reason, we inline them by
 * default. This macro is meant for internal use only, so that we can
 * also provide actual symbols for any caller that needs them.
 */
#if !defined(__BIONIC_CTYPE_INLINE)
#define __BIONIC_CTYPE_INLINE static __inline
#endif

/** Internal implementation detail. Do not use. */
#define _CTYPE_U 0x01
/** Internal implementation detail. Do not use. */
#define _CTYPE_L 0x02
/** Internal implementation detail. Do not use. */
#define _CTYPE_D 0x04
/** Internal implementation detail. Do not use. */
#define _CTYPE_S 0x08
/** Internal implementation detail. Do not use. */
#define _CTYPE_P 0x10
/** Internal implementation detail. Do not use. */
#define _CTYPE_C 0x20
/** Internal implementation detail. Do not use. */
#define _CTYPE_X 0x40
/** Internal implementation detail. Do not use. */
#define _CTYPE_B 0x80
/** Internal implementation detail. Do not use. */
#define _CTYPE_R (_CTYPE_P|_CTYPE_U|_CTYPE_L|_CTYPE_D|_CTYPE_B)
/** Internal implementation detail. Do not use. */
#define _CTYPE_A (_CTYPE_L|_CTYPE_U)
/** Internal implementation detail. Do not use. */
#define _CTYPE_N _CTYPE_D

__BEGIN_DECLS

/** Internal implementation detail. Do not use. */
extern const char* _ctype_;

/**
 * Returns the corresponding lower-case character if `ch` is upper-case, or undefined otherwise.
 *
 * Prefer tolower() instead.
 */
__BIONIC_CTYPE_INLINE int _tolower(int __ch) {
  return __ch | 0x20;
}

/**
 * Returns the corresponding upper-case character if `ch` is lower-case, or undefined otherwise.
 *
 * Prefer toupper() instead.
 */
__BIONIC_CTYPE_INLINE int _toupper(int __ch) {
  // Using EOR rather than AND makes no difference on arm, but saves an
  // instruction on arm64.
  return __ch ^ 0x20;
}

int isalnum(int __ch);
int isalpha(int __ch);
int isblank(int __ch);
int iscntrl(int __ch);
int isdigit(int __ch);
int isgraph(int __ch);
int islower(int __ch);
int isprint(int __ch);
int ispunct(int __ch);
int isspace(int __ch);
int isupper(int __ch);
int isxdigit(int __ch);
int tolower(int __ch);
int toupper(int __ch);

#if __ANDROID_API__ >= __ANDROID_API_L__
int isalnum_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
int isalpha_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
int isblank_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
int iscntrl_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
int isdigit_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
int isgraph_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
int islower_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
int isprint_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
int ispunct_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
int isspace_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
int isupper_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
int isxdigit_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
int tolower_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
int toupper_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
#else
/** Like isalnum() but with an ignored `locale_t`. */
__BIONIC_CTYPE_INLINE int isalnum_l(int __ch, locale_t __l) {
  return isalnum(__ch);
}

/** Like isalpha() but with an ignored `locale_t`. */
__BIONIC_CTYPE_INLINE int isalpha_l(int __ch, locale_t __l) {
  return isalpha(__ch);
}

/** Like isblank() but with an ignored `locale_t`. */
__BIONIC_CTYPE_INLINE int isblank_l(int __ch, locale_t __l) {
  return isblank(__ch);
}

/** Like iscntrl() but with an ignored `locale_t`. */
__BIONIC_CTYPE_INLINE int iscntrl_l(int __ch, locale_t __l) {
  return iscntrl(__ch);
}

/** Like isdigit() but with an ignored `locale_t`. */
__BIONIC_CTYPE_INLINE int isdigit_l(int __ch, locale_t __l) {
  return isdigit(__ch);
}

/** Like isgraph() but with an ignored `locale_t`. */
__BIONIC_CTYPE_INLINE int isgraph_l(int __ch, locale_t __l) {
  return isgraph(__ch);
}

/** Like islower() but with an ignored `locale_t`. */
__BIONIC_CTYPE_INLINE int islower_l(int __ch, locale_t __l) {
  return islower(__ch);
}

/** Like isprint() but with an ignored `locale_t`. */
__BIONIC_CTYPE_INLINE int isprint_l(int __ch, locale_t __l) {
  return isprint(__ch);
}

/** Like ispunct() but with an ignored `locale_t`. */
__BIONIC_CTYPE_INLINE int ispunct_l(int __ch, locale_t __l) {
  return ispunct(__ch);
}

/** Like isspace() but with an ignored `locale_t`. */
__BIONIC_CTYPE_INLINE int isspace_l(int __ch, locale_t __l) {
  return isspace(__ch);
}

/** Like isupper() but with an ignored `locale_t`. */
__BIONIC_CTYPE_INLINE int isupper_l(int __ch, locale_t __l) {
  return isupper(__ch);
}

/** Like isxdigit() but with an ignored `locale_t`. */
__BIONIC_CTYPE_INLINE int isxdigit_l(int __ch, locale_t __l) {
  return isxdigit(__ch);
}

/** Like tolower() but with an ignored `locale_t`. */
__BIONIC_CTYPE_INLINE int tolower_l(int __ch, locale_t __l) {
  return tolower(__ch);
}

/** Like toupper() but with an ignored `locale_t`. */
__BIONIC_CTYPE_INLINE int toupper_l(int __ch, locale_t __l) {
  return toupper(__ch);
}
#endif

int isascii(int __ch);
int toascii(int __ch);

__END_DECLS
