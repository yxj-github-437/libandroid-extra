/*
 * Copyright (C) 2008 The Android Open Source Project
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

#ifndef ANDROID_API_LEVEL_H
#define ANDROID_API_LEVEL_H

#include <sys/cdefs.h>

/*
 * Magic version number for a current development build, which has
 * not yet turned into an official release.
 */
#ifndef __ANDROID_API_FUTURE__
#define __ANDROID_API_FUTURE__ 10000
#endif

#ifndef __ANDROID_API__
#define __ANDROID_API__ 21//__ANDROID_API_FUTURE__
#else
#define __ANDROID_NDK__ 1
#endif

#define __ANDROID_API_G__ 9
#define __ANDROID_API_I__ 14
#define __ANDROID_API_J__ 16
#define __ANDROID_API_J_MR1__ 17
#define __ANDROID_API_J_MR2__ 18
#define __ANDROID_API_K__ 19
#define __ANDROID_API_L__ 21
#define __ANDROID_API_L_MR1__ 22
#define __ANDROID_API_M__ 23
#define __ANDROID_API_N__ 24
#define __ANDROID_API_N_MR1__ 25
#define __ANDROID_API_O__ 26
#define __ANDROID_API_O_MR1__ 27
#define __ANDROID_API_P__ 28

/* This file is included in <features.h>, and might be used from .S files. */
#if !defined(__ASSEMBLY__)

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#if __ANDROID_API__ < 29

/* android_get_device_api_level is a static inline before API level 29. */

// Avoid circular dependencies since this is exposed from <sys/cdefs.h>.
int __system_property_get(const char* __name, char* __value);
int atoi(const char* __s) __attribute__((__pure__));

static __inline__ __attribute__((__always_inline__))
int android_get_device_api_level() {
  char value[92] = { 0 };
  if (__system_property_get("ro.build.version.sdk", value) < 1) return -1;
  int api_level = atoi(value);
  return (api_level > 0) ? api_level : -1;
}

#else

/**
 * Returns the API level of the device we're actually running on, or -1 on failure.
 * The returned values correspond to the named constants in `<android/api-level.h>`,
 * and is equivalent to the Java `Build.VERSION.SDK_INT` API.
 *
 * See also android_get_application_target_sdk_version().
 */
int android_get_device_api_level() __INTRODUCED_IN(29);

#endif // __ANDROID_API__ < 29

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* defined(__ASSEMBLY__) */

#endif
