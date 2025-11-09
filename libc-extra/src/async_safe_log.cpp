/*
 * Copyright (C) 2010 The Android Open Source Project
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

#include "stdio_extra.h"
#include <android/set_abort_message.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/net.h>
#include <pthread.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>

#define BIONIC_DISALLOW_COPY_AND_ASSIGN(TypeName)                                                  \
    TypeName(const TypeName&) = delete;                                                            \
    void operator=(const TypeName&) = delete

class ErrnoRestorer
{
public:
    explicit ErrnoRestorer()
        : saved_errno_(errno)
    {}

    ~ErrnoRestorer()
    {
        errno = saved_errno_;
    }

    void override(int new_errno)
    {
        saved_errno_ = new_errno;
    }

private:
    int saved_errno_;

    BIONIC_DISALLOW_COPY_AND_ASSIGN(ErrnoRestorer);
};

// Don't call libc's close or socket, since it might call back into us as a result of fdsan/fdtrack.
#pragma GCC poison close
static int __close(int fd)
{
    return syscall(__NR_close, fd);
}

static int __socket(int domain, int type, int protocol)
{
#if defined(__i386__)
    unsigned long args[3] = {static_cast<unsigned long>(domain),
                             static_cast<unsigned long>(type),
                             static_cast<unsigned long>(protocol)};
    return syscall(__NR_socketcall, SYS_SOCKET, &args);
#else
    return syscall(__NR_socket, domain, type, protocol);
#endif
}

// Must be kept in sync with frameworks/base/core/java/android/util/EventLog.java.
enum AndroidEventLogType
{
    EVENT_TYPE_INT = 0,
    EVENT_TYPE_LONG = 1,
    EVENT_TYPE_STRING = 2,
    EVENT_TYPE_LIST = 3,
    EVENT_TYPE_FLOAT = 4,
};

__BEGIN_DECLS

int async_safe_format_buffer_va_list(char* buffer, size_t buffer_size, const char* format,
                                     va_list args)
{
    return vsnprintf(buffer, buffer_size, format, args);
}

int async_safe_format_buffer(char* buffer, size_t buffer_size, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    int buffer_len = async_safe_format_buffer_va_list(buffer, buffer_size, format, args);
    va_end(args);
    return buffer_len;
}

int async_safe_format_fd_va_list(int fd, const char* format, va_list args)
{
    return vdprintf(fd, format, args);
}

int async_safe_format_fd(int fd, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    int result = async_safe_format_fd_va_list(fd, format, args);
    va_end(args);
    return result;
}

static int write_stderr(const char* tag, const char* msg)
{
    iovec vec[4];
    vec[0].iov_base = const_cast<char*>(tag);
    vec[0].iov_len = strlen(tag);
    vec[1].iov_base = const_cast<char*>(": ");
    vec[1].iov_len = 2;
    vec[2].iov_base = const_cast<char*>(msg);
    vec[2].iov_len = strlen(msg);
    vec[3].iov_base = const_cast<char*>("\n");
    vec[3].iov_len = 1;

    int result = TEMP_FAILURE_RETRY(writev(STDERR_FILENO, vec, 4));
    return result;
}

static int open_log_socket()
{
    // ToDo: Ideally we want this to fail if the gid of the current
    // process is AID_LOGD, but will have to wait until we have
    // registered this in private/android_filesystem_config.h. We have
    // found that all logd crashes thus far have had no problem stuffing
    // the UNIX domain socket and moving on so not critical *today*.

    int log_fd =
        TEMP_FAILURE_RETRY(__socket(PF_UNIX, SOCK_DGRAM | SOCK_CLOEXEC | SOCK_NONBLOCK, 0));
    if (log_fd == -1) {
        return -1;
    }

    union
    {
        struct sockaddr addr;
        struct sockaddr_un addrUn;
    } u;
    memset(&u, 0, sizeof(u));
    u.addrUn.sun_family = AF_UNIX;
    strlcpy(u.addrUn.sun_path, "/dev/socket/logdw", sizeof(u.addrUn.sun_path));

    if (TEMP_FAILURE_RETRY(connect(log_fd, &u.addr, sizeof(u.addrUn))) != 0) {
        __close(log_fd);
        return -1;
    }

    return log_fd;
}

struct log_time
{// Wire format
    uint32_t tv_sec;
    uint32_t tv_nsec;
};

int async_safe_write_log(int priority, const char* tag, const char* msg)
{
    int main_log_fd = open_log_socket();
    if (main_log_fd == -1) {
        // Try stderr instead.
        return write_stderr(tag, msg);
    }

    iovec vec[6];
    char log_id = (priority == ANDROID_LOG_FATAL) ? LOG_ID_CRASH : LOG_ID_MAIN;
    vec[0].iov_base = &log_id;
    vec[0].iov_len = sizeof(log_id);
    uint16_t tid = gettid();
    vec[1].iov_base = &tid;
    vec[1].iov_len = sizeof(tid);
    timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    log_time realtime_ts;
    realtime_ts.tv_sec = ts.tv_sec;
    realtime_ts.tv_nsec = ts.tv_nsec;
    vec[2].iov_base = &realtime_ts;
    vec[2].iov_len = sizeof(realtime_ts);

    vec[3].iov_base = &priority;
    vec[3].iov_len = 1;
    vec[4].iov_base = const_cast<char*>(tag);
    vec[4].iov_len = strlen(tag) + 1;
    vec[5].iov_base = const_cast<char*>(msg);
    vec[5].iov_len = strlen(msg) + 1;

    int result = TEMP_FAILURE_RETRY(writev(main_log_fd, vec, sizeof(vec) / sizeof(vec[0])));
    __close(main_log_fd);
    return result;
}

int async_safe_format_log_va_list(int priority, const char* tag, const char* format, va_list args)
{
    ErrnoRestorer errno_restorer;
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);
    return async_safe_write_log(priority, tag, buffer);
}

int async_safe_format_log(int priority, const char* tag, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    int result = async_safe_format_log_va_list(priority, tag, format, args);
    va_end(args);
    return result;
}

void async_safe_fatal_va_list(const char* prefix, const char* format, va_list args)
{
    char msg[1024];
    size_t offset = 0;

    if (prefix) {
        offset += snprintf(msg, sizeof(msg), "%s: ", prefix);
    }

    vsnprintf(msg + offset, sizeof(msg) - offset, format, args);

    // Log to stderr for the benefit of "adb shell" users and gtests.
    struct iovec iov[2] = {
        {msg, strlen(msg)},
        {const_cast<char*>("\n"), 1},
    };
    TEMP_FAILURE_RETRY(writev(2, iov, 2));

    // Log to the log for the benefit of regular app developers (whose stdout and stderr are
    // closed).
    async_safe_write_log(ANDROID_LOG_FATAL, "libc", msg);

    android_set_abort_message(msg);
}

void async_safe_fatal_no_abort(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    async_safe_fatal_va_list(nullptr, fmt, args);
    va_end(args);
}

__END_DECLS
