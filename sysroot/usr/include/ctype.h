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
/** Internal implementation detail. Do not use. */
__attribute__((__no_sanitize__("unsigned-integer-overflow")))
__BIONIC_CTYPE_INLINE int __bionic_ctype_in_range(unsigned __lo, int __ch, unsigned __hi) {
  return (__BIONIC_CAST(static_cast, unsigned, __ch) - __lo) < (__hi - __lo + 1);
}

/** Like isalpha() but with an ignored `locale_t`.
    Returns true if `ch` is in `[A-Za-z]`. */
__BIONIC_CTYPE_INLINE int isalpha_l(int __ch, locale_t __l) {
  return __bionic_ctype_in_range('a', _tolower(__ch), 'z');
}

/** Like isblank() but with an ignored `locale_t`.
  * Returns true if `ch` is a space or tab. */
__BIONIC_CTYPE_INLINE int isblank_l(int __ch, locale_t __l) {
  return __ch == ' ' || __ch == '\t';
}

/** Like iscntrl() but with an ignored `locale_t`.
  * Returns true if `ch` is a control character (any character before space, plus DEL). */
__BIONIC_CTYPE_INLINE int iscntrl_l(int __ch, locale_t __l) {
  return (__BIONIC_CAST(static_cast, unsigned, __ch) < ' ') || __ch == 0x7f;
}

/** Like isdigit() but with an ignored `locale_t`.
  * Returns true if `ch` is in `[0-9]`. */
__BIONIC_CTYPE_INLINE int isdigit_l(int __ch, locale_t __l) {
  return __bionic_ctype_in_range('0', __ch, '9');
}

/** Like isalnum() but with an ignored `locale_t`.
  * Returns true if `ch` is in `[A-Za-z0-9]`. */
__BIONIC_CTYPE_INLINE int isalnum_l(int __ch, locale_t __l) {
  return isalpha_l(__ch, __l) || isdigit_l(__ch, __l);
}

/** Like isgraph() but with an ignored `locale_t`.
  * Returns true if `ch` is `[A-Za-z0-9]` or punctuation. */
__BIONIC_CTYPE_INLINE int isgraph_l(int __ch, locale_t __l) {
  return __bionic_ctype_in_range('!', __ch, '~');
}

/** Like islower() but with an ignored `locale_t`.
  * Returns true if `ch` is in `[a-z]`. */
__BIONIC_CTYPE_INLINE int islower_l(int __ch, locale_t __l) {
  return __bionic_ctype_in_range('a', __ch, 'z');
}

/** Like isprint() but with an ignored `locale_t`.
  * Returns true if `ch` is `[A-Za-z0-9]` or punctuation or space. */
__BIONIC_CTYPE_INLINE int isprint_l(int __ch, locale_t __l) {
  return __bionic_ctype_in_range(' ', __ch, '~');
}

/** Like ispunct() but with an ignored `locale_t`.
  * Returns true if `ch` is punctuation. */
__BIONIC_CTYPE_INLINE int ispunct_l(int __ch, locale_t __l) {
  return isgraph_l(__ch, __l) && !isalnum_l(__ch, __l);
}

/** Like isspace() but with an ignored `locale_t`.
  * Returns true if `ch` is in `[ \f\n\r\t\v]`. */
__BIONIC_CTYPE_INLINE int isspace_l(int __ch, locale_t __l) {
  return __ch == ' ' || __bionic_ctype_in_range('\t', __ch, '\r');
}

/** Like issupper() but with an ignored `locale_t`.
  * Returns true if `ch` is in `[A-Z]`. */
__BIONIC_CTYPE_INLINE int isupper_l(int __ch, locale_t __l) {
  return __bionic_ctype_in_range('A', __ch, 'Z');
}

/** Like isxdigit() but with an ignored `locale_t`.
  * Returns true if `ch` is in `[0-9A-Fa-f]`. */
__BIONIC_CTYPE_INLINE int isxdigit_l(int __ch, locale_t __l) {
  return isdigit_l(__ch, __l) || __bionic_ctype_in_range('a', _tolower(__ch), 'f') ;
}

/** Like tolower() but with an ignored `locale_t`.
  * Returns the corresponding lower-case character if `ch` is upper-case, or `ch` otherwise. */
__BIONIC_CTYPE_INLINE int tolower_l(int __ch, locale_t __l) {
  return (__bionic_ctype_in_range('A', __ch, 'Z')) ? _tolower(__ch) : __ch;
}

/** Like toupper() but with an ignored `locale_t`.
  * Returns the corresponding upper-case character if `ch` is lower-case, or `ch` otherwise. */
__BIONIC_CTYPE_INLINE int toupper_l(int __ch, locale_t __l) {
  return (__bionic_ctype_in_range('a', __ch, 'z')) ? _toupper(__ch) : __ch;
}
#endif

int isascii(int __ch);
int toascii(int __ch);

__END_DECLS
