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

#ifndef _STDIO_H_
#error "Never include this file directly; instead, include <stdio.h>"
#endif

char* _Nullable __fgets_chk(char* _Nonnull, int, FILE* _Nonnull, size_t);

#if __BIONIC_AVAILABILITY_GUARD(24)
size_t __fread_chk(void* _Nonnull, size_t, size_t, FILE* _Nonnull, size_t) __INTRODUCED_IN(24);
size_t __fwrite_chk(const void* _Nonnull, size_t, size_t, FILE* _Nonnull, size_t) __INTRODUCED_IN(24);
#endif /* __BIONIC_AVAILABILITY_GUARD(24) */


#if defined(__BIONIC_FORTIFY) && !defined(__BIONIC_NO_STDIO_FORTIFY)

#if __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
/* No diag -- clang diagnoses misuses of this on its own.  */
__BIONIC_FORTIFY_INLINE __printflike(3, 0) __diagnose_as_builtin(__builtin_vsnprintf, 1, 2, 3, 4)
int vsnprintf(char* const __BIONIC_COMPLICATED_NULLNESS __pass_object_size dest, size_t size, const char* _Nonnull format, va_list ap)
        __overloadable {
    return __builtin___vsnprintf_chk(dest, size, 0, __bos(dest), format, ap);
}

__BIONIC_FORTIFY_INLINE __printflike(2, 0)
int vsprintf(char* const __BIONIC_COMPLICATED_NULLNESS __pass_object_size dest, const char* _Nonnull format, va_list ap) __overloadable {
    return __builtin___vsprintf_chk(dest, 0, __bos(dest), format, ap);
}
#endif

#if defined(__clang__)
__BIONIC_ERROR_FUNCTION_VISIBILITY
int sprintf(char* __BIONIC_COMPLICATED_NULLNESS dest, const char* _Nonnull format)
    __overloadable
    __enable_if(__bos_unevaluated_lt(__bos(dest), __builtin_strlen(format)),
                "format string will always overflow destination buffer")
    __errorattr("format string will always overflow destination buffer");

#if __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
__BIONIC_FORTIFY_VARIADIC __printflike(2, 3)
int sprintf(char* const __BIONIC_COMPLICATED_NULLNESS __pass_object_size dest, const char* _Nonnull format, ...) __overloadable {
    va_list va;
    va_start(va, format);
    int result = __builtin___vsprintf_chk(dest, 0, __bos(dest), format, va);
    va_end(va);
    return result;
}

/* No diag -- clang diagnoses misuses of this on its own.  */
__BIONIC_FORTIFY_VARIADIC __printflike(3, 4) __diagnose_as_builtin(__builtin_snprintf, 1, 2, 3)
int snprintf(char* const __BIONIC_COMPLICATED_NULLNESS __pass_object_size dest, size_t size, const char* _Nonnull format, ...)
        __overloadable {
    va_list va;
    va_start(va, format);
    int result = __builtin___vsnprintf_chk(dest, size, 0, __bos(dest), format, va);
    va_end(va);
    return result;
}
#endif

#define __bos_trivially_ge_mul(bos_val, size, count) \
  __bos_dynamic_check_impl_and(bos_val, >=, (size) * (count), \
                               !__unsafe_check_mul_overflow(size, count))

__BIONIC_FORTIFY_INLINE
size_t fread(void* const _Nonnull __pass_object_size0 buf, size_t size, size_t count, FILE* _Nonnull stream)
        __overloadable
        __clang_error_if(__unsafe_check_mul_overflow(size, count),
                         "in call to 'fread', size * count overflows")
        __clang_error_if(__bos_unevaluated_lt(__bos0(buf), size * count),
                         "in call to 'fread', size * count is too large for the given buffer") {
#if __ANDROID_API__ >= 24 && __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    size_t bos = __bos0(buf);

    if (!__bos_trivially_ge_mul(bos, size, count)) {
        return __fread_chk(buf, size, count, stream, bos);
    }
#endif
    return __call_bypassing_fortify(fread)(buf, size, count, stream);
}

__BIONIC_FORTIFY_INLINE
size_t fwrite(const void* const _Nonnull __pass_object_size0 buf, size_t size, size_t count, FILE* _Nonnull stream)
        __overloadable
        __clang_error_if(__unsafe_check_mul_overflow(size, count),
                         "in call to 'fwrite', size * count overflows")
        __clang_error_if(__bos_unevaluated_lt(__bos0(buf), size * count),
                         "in call to 'fwrite', size * count is too large for the given buffer") {
#if __ANDROID_API__ >= 24 && __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    size_t bos = __bos0(buf);

    if (!__bos_trivially_ge_mul(bos, size, count)) {
        return __fwrite_chk(buf, size, count, stream, bos);
    }
#endif
    return __call_bypassing_fortify(fwrite)(buf, size, count, stream);
}
#undef __bos_trivially_ge_mul

__BIONIC_FORTIFY_INLINE
char* _Nullable fgets(char* const _Nonnull __pass_object_size dest, int size, FILE* _Nonnull stream)
        __overloadable
        __clang_error_if(size < 0, "in call to 'fgets', size should not be negative")
        __clang_error_if(__bos_unevaluated_lt(__bos(dest), size),
                         "in call to 'fgets', size is larger than the destination buffer") {
#if __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    size_t bos = __bos(dest);

    if (!__bos_dynamic_check_impl_and(bos, >=, (size_t)size, size >= 0)) {
        return __fgets_chk(dest, size, stream, bos);
    }
#endif
    return __call_bypassing_fortify(fgets)(dest, size, stream);
}

#else /* defined(__clang__) */

size_t __fread_real(void*, size_t, size_t, FILE*) __RENAME(fread);
__errordecl(__fread_too_big_error, "fread called with size * count bigger than buffer");
__errordecl(__fread_overflow, "fread called with overflowing size * count");

char* __fgets_real(char*, int, FILE*) __RENAME(fgets);
__errordecl(__fgets_too_big_error, "fgets called with size bigger than buffer");
__errordecl(__fgets_too_small_error, "fgets called with size less than zero");

size_t __fwrite_real(const void*, size_t, size_t, FILE*) __RENAME(fwrite);
__errordecl(__fwrite_too_big_error, "fwrite called with size * count bigger than buffer");
__errordecl(__fwrite_overflow, "fwrite called with overflowing size * count");


__BIONIC_FORTIFY_VARIADIC __printflike(3, 4)
int snprintf(char* dest, size_t size, const char* format, ...) {
    return __builtin___snprintf_chk(dest, size, 0, __bos(dest), format, __builtin_va_arg_pack());
}

__BIONIC_FORTIFY_VARIADIC __printflike(2, 3)
int sprintf(char* dest, const char* format, ...) {
    return __builtin___sprintf_chk(dest, 0, __bos(dest), format, __builtin_va_arg_pack());
}

__BIONIC_FORTIFY_INLINE
size_t fread(void* buf, size_t size, size_t count, FILE* stream) {
    size_t bos = __bos0(buf);

    if (__builtin_constant_p(size) && __builtin_constant_p(count)) {
        size_t total;
        if (__size_mul_overflow(size, count, &total)) {
            __fread_overflow();
        }

        if (total > bos) {
            __fread_too_big_error();
        }

        return __fread_real(buf, size, count, stream);
    }

#if __ANDROID_API__ >= 24 && __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    return __fread_chk(buf, size, count, stream, bos);
#endif

    return __fread_real(buf, size, count, stream);
}

__BIONIC_FORTIFY_INLINE
size_t fwrite(const void* buf, size_t size, size_t count, FILE* stream) {
    size_t bos = __bos0(buf);

    if (__builtin_constant_p(size) && __builtin_constant_p(count)) {
        size_t total;
        if (__size_mul_overflow(size, count, &total)) {
            __fwrite_overflow();
        }

        if (total > bos) {
            __fwrite_too_big_error();
        }

        return __fwrite_real(buf, size, count, stream);
    }

#if __ANDROID_API__ >= 24 && __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    return __fwrite_chk(buf, size, count, stream, bos);
#endif

    return __fwrite_real(buf, size, count, stream);
}

__BIONIC_FORTIFY_INLINE
char *fgets(char* dest, int size, FILE* stream) {
    size_t bos = __bos(dest);

    // Compiler can prove, at compile time, that the passed in size
    // is always negative. Force a compiler error.
    if (__builtin_constant_p(size) && (size < 0)) {
        __fgets_too_small_error();
    }

    // Compiler can prove, at compile time, that the passed in size
    // is always > the actual object size. Force a compiler error.
    if (__bos_unevaluated_lt(__bos(dest), size)) {
        __fgets_too_big_error();
    }


#if __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    // Compiler can prove, at compile time, that the passed in size
    // is always <= the actual object size. Don't call __fgets_chk
    if (!__bos_dynamic_check_impl_and(bos, >=, (size_t)size, size >= 0)) {
        return __fgets_chk(dest, size, stream, bos);
    }
#endif

    return __fgets_real(dest, size, stream);
}

#endif /* defined(__clang__) */
#endif /* defined(__BIONIC_FORTIFY) */
