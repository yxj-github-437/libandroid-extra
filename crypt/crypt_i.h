#ifndef CRYPT_H
#define CRYPT_H

#include <crypt.h>

#include <features.h>

#define weak_alias(old, new) \
    extern __typeof(old) new __attribute__((__weak__, __alias__(#old)))

__LIBC_HIDDEN__ char *__crypt_r(const char *, const char *, struct crypt_data *);

__LIBC_HIDDEN__ char *__crypt_des(const char *, const char *, char *);
__LIBC_HIDDEN__ char *__crypt_md5(const char *, const char *, char *);
__LIBC_HIDDEN__ char *__crypt_blowfish(const char *, const char *, char *);
__LIBC_HIDDEN__ char *__crypt_sha256(const char *, const char *, char *);
__LIBC_HIDDEN__ char *__crypt_sha512(const char *, const char *, char *);

#endif
