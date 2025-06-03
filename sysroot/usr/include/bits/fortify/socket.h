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

#ifndef _SYS_SOCKET_H_
#error "Never include this file directly; instead, include <sys/socket.h>"
#endif


#if __BIONIC_AVAILABILITY_GUARD(26)
ssize_t __sendto_chk(int, const void* _Nonnull, size_t, size_t, int, const struct sockaddr* _Nullable, socklen_t) __INTRODUCED_IN(26);
#endif /* __BIONIC_AVAILABILITY_GUARD(26) */

ssize_t __recvfrom_chk(int, void* _Nullable, size_t, size_t, int, struct sockaddr* _Nullable, socklen_t* _Nullable);

#if defined(__BIONIC_FORTIFY)

#if defined(__clang__)
__BIONIC_FORTIFY_INLINE
ssize_t recvfrom(int fd, void* _Nullable const buf __pass_object_size0, size_t len, int flags, struct sockaddr* _Nullable src_addr, socklen_t* _Nullable addr_len)
    __overloadable
    __clang_error_if(__bos_unevaluated_lt(__bos0(buf), len),
                     "'recvfrom' called with size bigger than buffer") {
#if __ANDROID_API__ >= 24 && __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
  size_t bos = __bos0(buf);

  if (!__bos_trivially_ge(bos, len)) {
    return __recvfrom_chk(fd, buf, len, bos, flags, src_addr, addr_len);
  }
#endif
  return __call_bypassing_fortify(recvfrom)(fd, buf, len, flags, src_addr, addr_len);
}

__BIONIC_FORTIFY_INLINE
ssize_t sendto(int fd, const void* _Nonnull const buf __pass_object_size0, size_t len, int flags, const struct sockaddr* _Nullable dest_addr, socklen_t addr_len)
    __overloadable
    __clang_error_if(__bos_unevaluated_lt(__bos0(buf), len),
                     "'sendto' called with size bigger than buffer") {
#if __ANDROID_API__ >= 26 && __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
  size_t bos = __bos0(buf);

  if (!__bos_trivially_ge(bos, len)) {
    return __sendto_chk(fd, buf, len, bos, flags, dest_addr, addr_len);
  }
#endif
  return __call_bypassing_fortify(sendto)(fd, buf, len, flags, dest_addr, addr_len);
}

__BIONIC_FORTIFY_INLINE
ssize_t recv(int socket, void* _Nullable const buf __pass_object_size0, size_t len, int flags)
    __overloadable
    __clang_error_if(__bos_unevaluated_lt(__bos0(buf), len),
                     "'recv' called with size bigger than buffer") {
  return recvfrom(socket, buf, len, flags, NULL, 0);
}

__BIONIC_FORTIFY_INLINE
ssize_t send(int socket, const void* _Nonnull const buf __pass_object_size0, size_t len, int flags)
    __overloadable
    __clang_error_if(__bos_unevaluated_lt(__bos0(buf), len),
                     "'send' called with size bigger than buffer") {
  return sendto(socket, buf, len, flags, NULL, 0);
}

#else /* defined(__clang__) */

ssize_t __recvfrom_real(int, void* const, size_t, int, struct sockaddr*, socklen_t*) __RENAME(recvform);
ssize_t __sendto_real(int, const void* const, size_t, int, const struct sockaddr*, socklen_t) __RENAME(sendto);

__errordecl(__recvfrom_size_too_bigger_error, "'recvfrom' called with size bigger than buffer");
__errordecl(__sendto_size_too_bigger_error, "'sendto' called with size bigger than buffer");
__errordecl(__recv_size_too_bigger_error, "'recv' called with size bigger than buffer");
__errordecl(__send_size_too_bigger_error, "'send' called with size bigger than buffer");

__BIONIC_FORTIFY_INLINE
ssize_t recvfrom(int fd, void* const buf __pass_object_size0, size_t len, int flags, struct sockaddr* src_addr, socklen_t* addr_len) {
  if(__bos_unevaluated_lt(__bos0(buf), len))
    __recvfrom_size_too_bigger_error();

#if __ANDROID_API__ >= 24 && __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
  size_t bos = __bos0(buf);

  if (!__bos_trivially_ge(bos, len)) {
    return __recvfrom_chk(fd, buf, len, bos, flags, src_addr, addr_len);
  }
#endif

  return __recvfrom_real(fd, buf, len, flags, src_addr, addr_len);
}

__BIONIC_FORTIFY_INLINE
ssize_t sendto(int fd, const void* const buf __pass_object_size0, size_t len, int flags, const struct sockaddr* dest_addr, socklen_t addr_len) {
  if(__bos_unevaluated_lt(__bos0(buf), len))
    __sendto_size_too_bigger_error();

#if __ANDROID_API__ >= 26 && __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
  size_t bos = __bos0(buf);

  if (!__bos_trivially_ge(bos, len)) {
    return __sendto_chk(fd, buf, len, bos, flags, dest_addr, addr_len);
  }
#endif

  return __sendto_real(fd, buf, len, flags, dest_addr, addr_len);
}

__BIONIC_FORTIFY_INLINE
ssize_t recv(int socket, void* const buf __pass_object_size0, size_t len, int flags) {
  if(__bos_unevaluated_lt(__bos0(buf), len))
    __recv_size_too_bigger_error();

  return recvfrom(socket, buf, len, flags, NULL, 0);
}

__BIONIC_FORTIFY_INLINE
ssize_t send(int socket, const void* const buf __pass_object_size0, size_t len, int flags) {
  if(__bos_unevaluated_lt(__bos0(buf), len))
    __send_size_too_bigger_error();

  return sendto(socket, buf, len, flags, NULL, 0);
}

#endif /* defined(__clang__) */
#endif /* __BIONIC_FORTIFY */
