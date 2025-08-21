/*
 * Copyright (C) 2017 The Android Open Source Project
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

#ifndef _STRING_H
#error "Never include this file directly; instead, include <string.h>"
#endif


#if __BIONIC_AVAILABILITY_GUARD(23)
void* _Nullable __memchr_chk(const void* _Nonnull, int, size_t, size_t) __INTRODUCED_IN(23);
void* _Nullable __memrchr_chk(const void* _Nonnull, int, size_t, size_t) __INTRODUCED_IN(23);
#endif /* __BIONIC_AVAILABILITY_GUARD(23) */

char* _Nonnull __stpncpy_chk2(char* _Nonnull, const char* _Nonnull, size_t, size_t, size_t);
char* _Nonnull __strncpy_chk2(char* _Nonnull, const char* _Nonnull, size_t, size_t, size_t);
size_t __strlcpy_chk(char* _Nonnull, const char* _Nonnull, size_t, size_t);
size_t __strlcat_chk(char* _Nonnull, const char* _Nonnull, size_t, size_t);

#if defined(__BIONIC_FORTIFY)
void* _Nullable __memrchr_real(const void* _Nonnull, int, size_t) __RENAME(memrchr);

#if __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
/* No diag -- clang diagnoses misuses of this on its own.  */
__BIONIC_FORTIFY_INLINE __diagnose_as_builtin(__builtin_memcpy, 1, 2, 3)
void* _Nonnull memcpy(void* _Nonnull const dst __pass_object_size0, const void* _Nonnull src, size_t copy_amount)
        __overloadable {
    return __builtin___memcpy_chk(dst, src, copy_amount, __bos0(dst));
}

/* No diag -- clang diagnoses misuses of this on its own.  */
__BIONIC_FORTIFY_INLINE __diagnose_as_builtin(__builtin_memmove, 1, 2, 3)
void* _Nonnull memmove(void* _Nonnull const dst __pass_object_size0, const void* _Nonnull src, size_t len)
        __overloadable {
    return __builtin___memmove_chk(dst, src, len, __bos0(dst));
}
#endif

#if defined(__clang__)
#if defined(__USE_GNU)
#if __ANDROID_API__ >= 30 || __ANDROID_EXTRA
__BIONIC_FORTIFY_INLINE __diagnose_as_builtin(__builtin_mempcpy, 1, 2, 3)
void* _Nonnull mempcpy(void* _Nonnull const dst __pass_object_size0, const void* _Nonnull src, size_t copy_amount)
        __overloadable
        __clang_error_if(__bos_unevaluated_lt(__bos0(dst), copy_amount),
                         "'mempcpy' called with size bigger than buffer") {
#if __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    size_t bos_dst = __bos0(dst);
    if (!__bos_trivially_ge(bos_dst, copy_amount)) {
        return __builtin___mempcpy_chk(dst, src, copy_amount, bos_dst);
    }
#endif
    return __builtin_mempcpy(dst, src, copy_amount);
}
#endif /* __ANDROID_API__ >= 30 */
#endif /* __USE_GNU */

__BIONIC_FORTIFY_INLINE
char* _Nonnull stpcpy(char* _Nonnull const dst __pass_object_size, const char* _Nonnull src)
        __overloadable
        __clang_error_if(__bos_unevaluated_le(__bos(dst), __builtin_strlen(src)),
                         "'stpcpy' called with string bigger than buffer") {
#if __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    return __builtin___stpcpy_chk(dst, src, __bos(dst));
#else
    return __builtin_stpcpy(dst, src);
#endif
}

__BIONIC_FORTIFY_INLINE __diagnose_as_builtin(__builtin_strcpy, 1, 2)
char* _Nonnull strcpy(char* _Nonnull const dst __pass_object_size, const char* _Nonnull src)
        __overloadable
        __clang_error_if(__bos_unevaluated_le(__bos(dst), __builtin_strlen(src)),
                         "'strcpy' called with string bigger than buffer") {
#if __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    return __builtin___strcpy_chk(dst, src, __bos(dst));
#else
    return __builtin_strcpy(dst, src);
#endif
}

__BIONIC_FORTIFY_INLINE
char* _Nonnull strcat(char* _Nonnull const dst __pass_object_size, const char* _Nonnull src)
        __overloadable
        __clang_error_if(__bos_unevaluated_le(__bos(dst), __builtin_strlen(src)),
                         "'strcat' called with string bigger than buffer") {
#if __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    return __builtin___strcat_chk(dst, src, __bos(dst));
#else
    return __builtin_strcat(dst, src);
#endif
}

#else /* defined(__clang__) */

__errordecl(__mempcpy_dest_size_error, "'mempcpy' called with size bigger than buffer");
__errordecl(__stpcpy_dest_size_error, "'stpcpy' called with string bigger than buffer");
__errordecl(__strcpy_dest_size_error, "'strcpy' called with string bigger than buffer");
__errordecl(__strcat_dest_size_error, "'strcat' called with size bigger than buffer");

#if defined(__USE_GNU)
#if __ANDROID_API__ >= 30 || __ANDROID_EXTRA
__BIONIC_FORTIFY_INLINE __diagnose_as_builtin(__builtin_mempcpy, 1, 2, 3)
void* mempcpy(void* const dst __pass_object_size0, const void* src, size_t copy_amount) {
    if(__bos_unevaluated_lt(__bos0(dst), copy_amount)) {
        __mempcpy_dest_size_error();
    }

#if __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    size_t bos_dst = __bos0(dst);
    if (!__bos_trivially_ge(bos_dst, copy_amount)) {
        return __builtin___mempcpy_chk(dst, src, copy_amount, bos_dst);
    }
#endif
    return __builtin_mempcpy(dst, src, copy_amount);
}
#endif /* __ANDROID_API__ >= 30 */
#endif /* __USE_GNU */

__BIONIC_FORTIFY_INLINE
char* stpcpy(char* const dst __pass_object_size, const char* src) {
    if(__bos0(src) != __BIONIC_FORTIFY_UNKNOWN_SIZE &&
       __bos_unevaluated_le(__bos(dst), __builtin_strlen(src))) {
        __stpcpy_dest_size_error();
    }

#if __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    return __builtin___stpcpy_chk(dst, src, __bos(dst));
#else
    return __builtin_stpcpy(dst, src);
#endif
}

__BIONIC_FORTIFY_INLINE __diagnose_as_builtin(__builtin_strcpy, 1, 2)
char* strcpy(char* const dst __pass_object_size, const char* src) {
    if(__bos0(src) != __BIONIC_FORTIFY_UNKNOWN_SIZE &&
       __bos_unevaluated_le(__bos(dst), __builtin_strlen(src))) {
        __strcpy_dest_size_error();
    }

#if __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    return __builtin___strcpy_chk(dst, src, __bos(dst));
#else
    return __builtin_strcpy(dst, src);
#endif
}

__BIONIC_FORTIFY_INLINE
char* strcat(char* const dst __pass_object_size, const char* src) {
    if(__bos0(src) != __BIONIC_FORTIFY_UNKNOWN_SIZE &&
       __bos_unevaluated_le(__bos(dst), __builtin_strlen(src))) {
        __strcat_dest_size_error();
    }

#if __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    return __builtin___strcat_chk(dst, src, __bos(dst));
#else
    return __builtin_strcat(dst, src);
#endif
}

#endif /* defined(__clang__) */

#if __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
/* No diag -- clang diagnoses misuses of this on its own.  */
__BIONIC_FORTIFY_INLINE __diagnose_as_builtin(__builtin_strncat, 1, 2, 3)
char* _Nonnull strncat(char* _Nonnull const dst __pass_object_size, const char* _Nonnull src, size_t n)
       __overloadable {
    return __builtin___strncat_chk(dst, src, n, __bos(dst));
}
#endif

/* No diag -- clang diagnoses misuses of this on its own.  */
__BIONIC_FORTIFY_INLINE __diagnose_as_builtin(__builtin_memset, 1, 2, 3)
void* _Nonnull memset(void* _Nonnull const s __pass_object_size0, int c, size_t n) __overloadable
#if defined(__clang__)
        /* If you're a user who wants this warning to go away: use `(&memset)(foo, bar, baz)`. */
        __clang_warning_if(c && !n, "'memset' will set 0 bytes; maybe the arguments got flipped?")
#endif
{
#if __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    return __builtin___memset_chk(s, c, n, __bos0(s));
#else
    return __builtin_memset(s, c, n);
#endif
}

#if __ANDROID_API__ >= 23 && __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
__BIONIC_FORTIFY_INLINE
void* _Nullable memchr(const void* _Nonnull const s __pass_object_size, int c, size_t n) __overloadable {
    size_t bos = __bos(s);

    if (__bos_trivially_ge(bos, n)) {
        return __builtin_memchr(s, c, n);
    }

    return __memchr_chk(s, c, n, bos);
}

__BIONIC_FORTIFY_INLINE
void* _Nullable __memrchr_fortify(const void* _Nonnull const __pass_object_size s, int c, size_t n) __overloadable {
    size_t bos = __bos(s);

    if (__bos_trivially_ge(bos, n)) {
        return __memrchr_real(s, c, n);
    }

    return __memrchr_chk(s, c, n, bos);
}
#endif

#if __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
/* No diag -- clang diagnoses misuses of this on its own.  */
__BIONIC_FORTIFY_INLINE __diagnose_as_builtin(__builtin_stpncpy, 1, 2, 3)
char* _Nonnull stpncpy(char* _Nonnull const dst __pass_object_size, const char* _Nonnull const src __pass_object_size, size_t n)
        __overloadable {
    size_t bos_dst = __bos(dst);
    size_t bos_src = __bos(src);

    /* Ignore dst size checks; they're handled in strncpy_chk */
    if (bos_src == __BIONIC_FORTIFY_UNKNOWN_SIZE) {
        return __builtin___stpncpy_chk(dst, src, n, bos_dst);
    }

    return __stpncpy_chk2(dst, src, n, bos_dst, bos_src);
}

/* No diag -- clang diagnoses misuses of this on its own.  */
__BIONIC_FORTIFY_INLINE __diagnose_as_builtin(__builtin_strncpy, 1, 2, 3)
char* _Nonnull strncpy(char* _Nonnull const dst __pass_object_size, const char* _Nonnull const src __pass_object_size, size_t n)
        __overloadable {
    size_t bos_dst = __bos(dst);
    size_t bos_src = __bos(src);

    /* Ignore dst size checks; they're handled in strncpy_chk */
    if (bos_src == __BIONIC_FORTIFY_UNKNOWN_SIZE) {
        return __builtin___strncpy_chk(dst, src, n, bos_dst);
    }

    return __strncpy_chk2(dst, src, n, bos_dst, bos_src);
}
#endif

#if defined(__clang__)
__BIONIC_FORTIFY_INLINE
size_t strlcpy(char* _Nonnull const dst __pass_object_size, const char* _Nonnull src, size_t size)
        __overloadable
        __clang_error_if(__bos_unevaluated_lt(__bos(dst), size),
                         "'strlcpy' called with size bigger than buffer") {
#if __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    return __strlcpy_chk(dst, src, size, __bos(dst));
#else
    return __call_bypassing_fortify(strlcpy)(dst, src, size);
#endif
}

__BIONIC_FORTIFY_INLINE
size_t strlcat(char* _Nonnull const dst __pass_object_size, const char* _Nonnull src, size_t size)
        __overloadable
        __clang_error_if(__bos_unevaluated_lt(__bos(dst), size),
                         "'strlcat' called with size bigger than buffer") {
#if __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    return __strlcat_chk(dst, src, size, __bos(dst));
#else
    return __call_bypassing_fortify(strlcat)(dst, src, size);
#endif
}

#else /* defined(__clang__) */

size_t __strlcpy_real(char* const dst __pass_object_size, const char* src, size_t size) __RENAME(strlcpy);
__errordecl(__strlcpy_dest_size_error, "'strlcpy' called with size bigger than buffer");

size_t __strlcat_real(char* const dst __pass_object_size, const char* src, size_t size) __RENAME(strlcat);
__errordecl(__strlcat_dest_size_error, "'strlcat' called with size bigger than buffer");

__BIONIC_FORTIFY_INLINE
size_t strlcpy(char* const dst __pass_object_size, const char* src, size_t size) {
    if(__bos_unevaluated_lt(__bos(dst), size)) {
        __strlcpy_dest_size_error();
    }

#if __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    return __strlcpy_chk(dst, src, size, __bos(dst));
#else
    return __strlcpy_real(dst, src, size);
#endif
}

__BIONIC_FORTIFY_INLINE
size_t strlcat(char* const dst __pass_object_size, const char* src, size_t size) {
    if(__bos_unevaluated_lt(__bos(dst), size)) {
        __strlcat_dest_size_error();
    }

#if __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    return __strlcat_chk(dst, src, size, __bos(dst));
#else
    return __strlcat_real(dst, src, size);
#endif
}
#endif /* defined(__clang__) */

#if __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
/*
 * Clang, when parsing C, can fold strlen to a constant without LLVM's help.
 * This doesn't apply to overloads of strlen, so write this differently. We
 * can't use `__pass_object_size0` here, but that's fine: it doesn't help much
 * on __always_inline functions.
 */
extern __always_inline __inline__ __attribute__((gnu_inline)) size_t strlen(const char* _Nonnull s) {
    return __strlen_chk(s, __bos0(s));
}
#endif

__BIONIC_FORTIFY_INLINE
char* _Nullable strchr(const char* _Nonnull const s __pass_object_size, int c) __overloadable {
#if __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    size_t bos = __bos(s);

    if (bos != __BIONIC_FORTIFY_UNKNOWN_SIZE) {
        return __strchr_chk(s, c, bos);
    }
#endif
    return __builtin_strchr(s, c);
}

__BIONIC_FORTIFY_INLINE
char* _Nullable strrchr(const char* _Nonnull const s __pass_object_size, int c) __overloadable {
#if __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    size_t bos = __bos(s);

    if (bos != __BIONIC_FORTIFY_UNKNOWN_SIZE) {
        return __strrchr_chk(s, c, bos);
    }
#endif
    return __builtin_strrchr(s, c);
}

#if __ANDROID_API__ >= 23 && __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
#if defined(__cplusplus)
extern "C++" {
__BIONIC_FORTIFY_INLINE
void* _Nullable memrchr(void* _Nonnull const __pass_object_size s, int c, size_t n) {
    return __memrchr_fortify(s, c, n);
}

__BIONIC_FORTIFY_INLINE
const void* _Nullable memrchr(const void* _Nonnull const __pass_object_size s, int c, size_t n) {
    return __memrchr_fortify(s, c, n);
}
}
#else
__BIONIC_FORTIFY_INLINE
void* _Nullable memrchr(const void* _Nonnull const __pass_object_size s, int c, size_t n) __overloadable {
    return __memrchr_fortify(s, c, n);
}
#endif
#endif /* __ANDROID_API__ >= 23 */

#endif /* defined(__BIONIC_FORTIFY) */
