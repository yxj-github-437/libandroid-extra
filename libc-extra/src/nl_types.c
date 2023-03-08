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

#include <errno.h>
#include <nl_types.h>

/**
 * [catopen(3)](http://man7.org/linux/man-pages/man3/catopen.3.html) opens a message catalog.
 *
 * On Android, this always returns failure: `((nl_catd) -1)`.
 *
 * Available since API level 28.
 */

__attribute__((visibility("hidden"))) nl_catd catopen(const char*, int)
{
    return (nl_catd)(-1);
}

/**
 * [catgets(3)](http://man7.org/linux/man-pages/man3/catgets.3.html) translates the given message
 * using the given message catalog.
 *
 * On Android, this always returns `__msg`.
 *
 * Available since API level 28.
 */
__attribute__((visibility("hidden"))) char* catgets(nl_catd, int, int, const char* message)
{
    return (char*)(message);
}

/**
 * [catclose(3)](http://man7.org/linux/man-pages/man3/catclose.3.html) closes a message catalog.
 *
 * On Android, this always returns -1 with `errno` set to `EBADF`.
 */
__attribute__((visibility("hidden"))) int catclose(nl_catd)
{
    // Since we didn't hand out a valid nl_catd, you can't be returning one to us.
    errno = EBADF;
    return -1;
}
