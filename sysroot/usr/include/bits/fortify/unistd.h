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
#ifndef _UNISTD_H_
#error "Never include this file directly; instead, include <unistd.h>"
#endif


#if __BIONIC_AVAILABILITY_GUARD(24)
char* _Nullable __getcwd_chk(char* _Nullable, size_t, size_t) __INTRODUCED_IN(24);
#endif /* __BIONIC_AVAILABILITY_GUARD(24) */



#if __BIONIC_AVAILABILITY_GUARD(23)
ssize_t __pread_chk(int, void* _Nonnull, size_t, off_t, size_t) __INTRODUCED_IN(23);
#endif /* __BIONIC_AVAILABILITY_GUARD(23) */

ssize_t __pread_real(int, void* _Nonnull, size_t, off_t) __RENAME(pread);


#if __BIONIC_AVAILABILITY_GUARD(23)
ssize_t __pread64_chk(int, void* _Nonnull, size_t, off64_t, size_t) __INTRODUCED_IN(23);
#endif /* __BIONIC_AVAILABILITY_GUARD(23) */

ssize_t __pread64_real(int, void* _Nonnull, size_t, off64_t) __RENAME(pread64);


#if __BIONIC_AVAILABILITY_GUARD(24)
ssize_t __pwrite_chk(int, const void* _Nonnull, size_t, off_t, size_t) __INTRODUCED_IN(24);
#endif /* __BIONIC_AVAILABILITY_GUARD(24) */

ssize_t __pwrite_real(int, const void* _Nonnull, size_t, off_t) __RENAME(pwrite);


#if __BIONIC_AVAILABILITY_GUARD(24)
ssize_t __pwrite64_chk(int, const void* _Nonnull, size_t, off64_t, size_t) __INTRODUCED_IN(24);
#endif /* __BIONIC_AVAILABILITY_GUARD(24) */

ssize_t __pwrite64_real(int, const void* _Nonnull, size_t, off64_t) __RENAME(pwrite64);

ssize_t __read_chk(int, void* __BIONIC_COMPLICATED_NULLNESS, size_t, size_t);

#if __BIONIC_AVAILABILITY_GUARD(24)
ssize_t __write_chk(int, const void* __BIONIC_COMPLICATED_NULLNESS, size_t, size_t) __INTRODUCED_IN(24);
#endif /* __BIONIC_AVAILABILITY_GUARD(24) */


#if __BIONIC_AVAILABILITY_GUARD(23)
ssize_t __readlink_chk(const char* _Nonnull, char* _Nonnull, size_t, size_t) __INTRODUCED_IN(23);
ssize_t __readlinkat_chk(int dirfd, const char* _Nonnull, char* _Nonnull, size_t, size_t) __INTRODUCED_IN(23);
#endif /* __BIONIC_AVAILABILITY_GUARD(23) */


#if defined(__BIONIC_FORTIFY)

#if defined(__USE_FILE_OFFSET64)
#define __PREAD_PREFIX(x) __pread64_ ## x
#define __PWRITE_PREFIX(x) __pwrite64_ ## x
#else
#define __PREAD_PREFIX(x) __pread_ ## x
#define __PWRITE_PREFIX(x) __pwrite_ ## x
#endif

#define __bos_trivially_ge_no_overflow(bos_val, index)  \
      ((__bos_dynamic_check_impl_and((bos_val), >=, (index), (bos_val) <= SSIZE_MAX) && \
        __builtin_constant_p(index) && (index) <= SSIZE_MAX))

#if defined(__clang__)
#define __error_if_overflows_ssizet(what, fn) \
    __clang_error_if((what) > SSIZE_MAX, "in call to '" #fn "', '" #what "' must be <= SSIZE_MAX")

#define __error_if_overflows_objectsize(what, objsize, fn) \
    __clang_error_if(__bos_unevaluated_lt((objsize), (what)), \
                     "in call to '" #fn "', '" #what "' bytes overflows the given object")


__BIONIC_FORTIFY_INLINE
char* _Nullable getcwd(char* const _Nullable __pass_object_size buf, size_t size)
        __overloadable
        __error_if_overflows_objectsize(size, __bos(buf), getcwd) {
#if __ANDROID_API__ >= 24 && __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    size_t bos = __bos(buf);

    if (!__bos_trivially_ge(bos, size)) {
        return __getcwd_chk(buf, size, bos);
    }
#endif
    return __call_bypassing_fortify(getcwd)(buf, size);
}

#if !defined(__USE_FILE_OFFSET64)
__BIONIC_FORTIFY_INLINE
ssize_t pread(int fd, void* const _Nonnull __pass_object_size0 buf, size_t count, off_t offset)
        __overloadable
        __error_if_overflows_ssizet(count, pread)
        __error_if_overflows_objectsize(count, __bos0(buf), pread) {
#if __ANDROID_API__ >= 23 && __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    size_t bos = __bos0(buf);

    if (!__bos_trivially_ge_no_overflow(bos, count)) {
        return __PREAD_PREFIX(chk)(fd, buf, count, offset, bos);
    }
#endif
    return __PREAD_PREFIX(real)(fd, buf, count, offset);
}
#endif /* !defined(__USE_FILE_OFFSET64) */

__BIONIC_FORTIFY_INLINE
ssize_t pread64(int fd, void* const _Nonnull __pass_object_size0 buf, size_t count, off64_t offset)
        __overloadable
        __error_if_overflows_ssizet(count, pread64)
        __error_if_overflows_objectsize(count, __bos0(buf), pread64) {
#if __ANDROID_API__ >= 23 && __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    size_t bos = __bos0(buf);

    if (!__bos_trivially_ge_no_overflow(bos, count)) {
        return __pread64_chk(fd, buf, count, offset, bos);
    }
#endif
    return __pread64_real(fd, buf, count, offset);
}

#if !defined(__USE_FILE_OFFSET64)
__BIONIC_FORTIFY_INLINE
ssize_t pwrite(int fd, const void* const _Nonnull __pass_object_size0 buf, size_t count, off_t offset)
        __overloadable
        __error_if_overflows_ssizet(count, pwrite)
        __error_if_overflows_objectsize(count, __bos0(buf), pwrite) {
#if __ANDROID_API__ >= 24 && __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    size_t bos = __bos0(buf);

    if (!__bos_trivially_ge_no_overflow(bos, count)) {
        return __PWRITE_PREFIX(chk)(fd, buf, count, offset, bos);
    }
#endif
    return __PWRITE_PREFIX(real)(fd, buf, count, offset);
}
#endif /* !defined(__USE_FILE_OFFSET64) */

__BIONIC_FORTIFY_INLINE
ssize_t pwrite64(int fd, const void* const _Nonnull __pass_object_size0 buf, size_t count, off64_t offset)
        __overloadable
        __error_if_overflows_ssizet(count, pwrite64)
        __error_if_overflows_objectsize(count, __bos0(buf), pwrite64) {
#if __ANDROID_API__ >= 24 && __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    size_t bos = __bos0(buf);

    if (!__bos_trivially_ge_no_overflow(bos, count)) {
        return __pwrite64_chk(fd, buf, count, offset, bos);
    }
#endif
    return __pwrite64_real(fd, buf, count, offset);
}

__BIONIC_FORTIFY_INLINE
ssize_t read(int fd, void* const __BIONIC_COMPLICATED_NULLNESS __pass_object_size0 buf, size_t count)
        __overloadable
        __error_if_overflows_ssizet(count, read)
        __error_if_overflows_objectsize(count, __bos0(buf), read) {
#if __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    size_t bos = __bos0(buf);

    if (!__bos_trivially_ge_no_overflow(bos, count)) {
        return __read_chk(fd, buf, count, bos);
    }
#endif
    return __call_bypassing_fortify(read)(fd, buf, count);
}

__BIONIC_FORTIFY_INLINE
ssize_t write(int fd, const void* const __BIONIC_COMPLICATED_NULLNESS __pass_object_size0 buf, size_t count)
        __overloadable
        __error_if_overflows_ssizet(count, write)
        __error_if_overflows_objectsize(count, __bos0(buf), write) {
#if __ANDROID_API__ >= 24 && __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    size_t bos = __bos0(buf);

    if (!__bos_trivially_ge_no_overflow(bos, count)) {
        return __write_chk(fd, buf, count, bos);
    }
#endif
    return __call_bypassing_fortify(write)(fd, buf, count);
}

__BIONIC_FORTIFY_INLINE
ssize_t readlink(const char* _Nonnull path, char* _Nonnull const __pass_object_size buf, size_t size)
        __overloadable
        __error_if_overflows_ssizet(size, readlink)
        __error_if_overflows_objectsize(size, __bos(buf), readlink) {
#if __ANDROID_API__ >= 23 && __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    size_t bos = __bos(buf);

    if (!__bos_trivially_ge_no_overflow(bos, size)) {
        return __readlink_chk(path, buf, size, bos);
    }
#endif
    return __call_bypassing_fortify(readlink)(path, buf, size);
}

__BIONIC_FORTIFY_INLINE
ssize_t readlinkat(int dirfd, const char* _Nonnull path, char* const _Nonnull __pass_object_size buf, size_t size)
        __overloadable
        __error_if_overflows_ssizet(size, readlinkat)
        __error_if_overflows_objectsize(size, __bos(buf), readlinkat) {
#if __ANDROID_API__ >= 23 && __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    size_t bos = __bos(buf);

    if (!__bos_trivially_ge_no_overflow(bos, size)) {
        return __readlinkat_chk(dirfd, path, buf, size, bos);
    }
#endif
    return __call_bypassing_fortify(readlinkat)(dirfd, path, buf, size);
}

#undef __enable_if_no_overflow_ssizet
#undef __error_if_overflows_objectsize
#undef __error_if_overflows_ssizet
#else /* defined(__clang__) */

char* __getcwd_real(char*, size_t) __RENAME(getcwd);
ssize_t __read_real(int, void*, size_t) __RENAME(read);
ssize_t __write_real(int, const void*, size_t) __RENAME(write);
ssize_t __readlink_real(const char*, char*, size_t) __RENAME(readlink);
ssize_t __readlinkat_real(int dirfd, const char*, char*, size_t) __RENAME(readlinkat);

__errordecl(__getcwd_dest_size_error, "getcwd called with size bigger than destination");
__errordecl(__pread_dest_size_error, "pread called with size bigger than destination");
__errordecl(__pread_count_toobig_error, "pread called with count > SSIZE_MAX");
__errordecl(__pread64_dest_size_error, "pread64 called with size bigger than destination");
__errordecl(__pread64_count_toobig_error, "pread64 called with count > SSIZE_MAX");
__errordecl(__pwrite_dest_size_error, "pwrite called with size bigger than destination");
__errordecl(__pwrite_count_toobig_error, "pwrite called with count > SSIZE_MAX");
__errordecl(__pwrite64_dest_size_error, "pwrite64 called with size bigger than destination");
__errordecl(__pwrite64_count_toobig_error, "pwrite64 called with count > SSIZE_MAX");
__errordecl(__read_dest_size_error, "read called with size bigger than destination");
__errordecl(__read_count_toobig_error, "read called with count > SSIZE_MAX");
__errordecl(__write_dest_size_error, "write called with size bigger than destination");
__errordecl(__write_count_toobig_error, "write called with count > SSIZE_MAX");
__errordecl(__readlink_dest_size_error, "readlink called with size bigger than destination");
__errordecl(__readlink_size_toobig_error, "readlink called with size > SSIZE_MAX");
__errordecl(__readlinkat_dest_size_error, "readlinkat called with size bigger than destination");
__errordecl(__readlinkat_size_toobig_error, "readlinkat called with size > SSIZE_MAX");

#define __overflows_ssizet(what) ((what) > SSIZE_MAX)
#define __overflows_objectsize(what, objsize) \
    (__bos_unevaluated_lt((objsize), (what)))

__BIONIC_FORTIFY_INLINE
char* getcwd(char* buf, size_t size) __overloadable {
    if (__overflows_objectsize(size, __bos(buf))) {
        __getcwd_dest_size_error();
    }

#if __ANDROID_API__ >= 24 && __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    size_t bos = __bos(buf)
    if (!__bos_trivially_ge(bos, size)) {
        return __getcwd_chk(buf, size, bos);
    }
#endif

    return __getcwd_real(buf, size);
}

#if !defined(__USE_FILE_OFFSET64)
__BIONIC_FORTIFY_INLINE
ssize_t pread(int fd, void* buf, size_t count, off_t offset) {
    if (__overflows_ssizet(count)) {
        __PREAD_PREFIX(count_toobig_error)();
    }

    if (__overflows_objectsize(count, __bos0(buf))) {
        __PREAD_PREFIX(dest_size_error)();
    }

#if __ANDROID_API__ >= 23 && __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    size_t bos = __bos0(buf);

    if (!__bos_trivially_ge_no_overflow(bos, count)) {
        return __PREAD_PREFIX(chk)(fd, buf, count, offset, bos);
    }
#endif

    return __PREAD_PREFIX(real)(fd, buf, count, offset);
}
#endif /* !defined(__USE_FILE_OFFSET64) */

__BIONIC_FORTIFY_INLINE
ssize_t pread64(int fd, void* buf, size_t count, off64_t offset) {
    if (__overflows_ssizet(count)) {
        __pread64_count_toobig_error();
    }

    if (__overflows_objectsize(count, __bos0(buf))) {
        __pread64_dest_size_error();
    }

#if __ANDROID_API__ >= 23 && __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    size_t bos = __bos0(buf);

    if (!__bos_trivially_ge_no_overflow(bos, count)) {
        return __pread64_chk(fd, buf, count, offset, bos);
    }
#endif

    return __pread64_real(fd, buf, count, offset);
}

#if !defined(__USE_FILE_OFFSET64)
__BIONIC_FORTIFY_INLINE
ssize_t pwrite(int fd, const void* buf, size_t count, off_t offset) {
    if (__overflows_ssizet(count)) {
        __PWRITE_PREFIX(count_toobig_error)();
    }

    if (__overflows_objectsize(count, __bos0(buf))) {
        __PWRITE_PREFIX(dest_size_error)();
    }

#if __ANDROID_API__ >= 24 && __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    size_t bos = __bos0(buf);

    if (!__bos_trivially_ge_no_overflow(bos, count)) {
        return __PWRITE_PREFIX(chk)(fd, buf, count, offset, bos);
    }
#endif

    return __PWRITE_PREFIX(real)(fd, buf, count, offset);
}
#endif /* !defined(__USE_FILE_OFFSET64) */

__BIONIC_FORTIFY_INLINE
ssize_t pwrite64(int fd, const void* buf, size_t count, off64_t offset) {
    if (__overflows_ssizet(count)) {
        __pwrite64_count_toobig_error();
    }

    if (__overflows_objectsize(count, __bos0(buf))) {
        __pwrite64_dest_size_error();
    }

#if __ANDROID_API__ >= 24 && __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    size_t bos = __bos0(buf);

    if (!__bos_trivially_ge_no_overflow(bos, count)) {
        return __pwrite64_chk(fd, buf, count, offset, bos);
    }
#endif

    return __pwrite64_real(fd, buf, count, offset);
}

__BIONIC_FORTIFY_INLINE
ssize_t read(int fd, void* buf, size_t count) {
    if (__overflows_ssizet(count)) {
        __read_count_toobig_error();
    }

    if (__overflows_objectsize(count, __bos0(buf))) {
        __read_dest_size_error();
    }

#if __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    size_t bos = __bos0(buf);

    if (!__bos_trivially_ge_no_overflow(bos, count)) {
        return __read_chk(fd, buf, count, bos);
    }
#endif

    return __read_real(fd, buf, count);
}

__BIONIC_FORTIFY_INLINE
ssize_t write(int fd, const void* buf, size_t count) {
    if (__overflows_ssizet(count)) {
        __write_count_toobig_error();
    }

    if (__overflows_objectsize(count, __bos0(buf))) {
        __write_dest_size_error();
    }

#if __ANDROID_API__ >= 24 && __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    size_t bos = __bos0(buf);

    if (!__bos_trivially_ge_no_overflow(bos, count)) {
        return __write_chk(fd, buf, count, bos);
    }
#endif

    return __write_real(fd, buf, count);
}

__BIONIC_FORTIFY_INLINE
ssize_t readlink(const char* path, char* buf, size_t size) {
    if (__overflows_ssizet(size)) {
        __readlink_size_toobig_error();
    }
    if (__overflows_objectsize(size, __bos0(buf))) {
        __readlink_dest_size_error();
    }

#if __ANDROID_API__ >= 23 && __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    size_t bos = __bos(buf);

    if (!__bos_trivially_ge_no_overflow(bos, size)) {
        return __readlink_chk(path, buf, size, bos);
    }
#endif

    return __readlink_real(path, buf, size);
}

__BIONIC_FORTIFY_INLINE
ssize_t readlinkat(int dirfd, const char* path, char* buf, size_t size) {
    if (__overflows_ssizet(size)) {
        __readlinkat_size_toobig_error();
    }

    if (__overflows_objectsize(size, __bos0(buf))) {
        __readlinkat_dest_size_error();
    }

#if __ANDROID_API__ >= 23 && __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    size_t bos = __bos(buf);

    if (!__bos_trivially_ge_no_overflow(bos, size)) {
        return __readlinkat_chk(path, buf, size, bos);
    }
#endif

    return __readlinkat_real(dirfd, path, buf, size);
}

#undef __overflows_ssizet
#undef __overflows_objectsize
#endif /* defined(__clang__) */

#undef __bos_trivially_ge_no_overflow
#undef __PREAD_PREFIX
#undef __PWRITE_PREFIX
#endif /* defined(__BIONIC_FORTIFY) */
