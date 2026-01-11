#ifndef _STRING_H
#error "Never include this file directly; instead, include <string.h>"
#endif

#ifdef __ANDROID_EXTRA

__BEGIN_DECLS

#if defined(__USE_GNU)
void* _Nonnull mempcpy(void* _Nonnull __dst, const void* _Nonnull __src, size_t __n);

#if defined(__cplusplus)
extern "C++" char* _Nonnull strchrnul(char* _Nonnull __s, int __ch) __RENAME(strchrnul) __attribute_pure__;
extern "C++" const char* _Nonnull strchrnul(const char* _Nonnull __s, int __ch) __RENAME(strchrnul) __attribute_pure__;
#else
char* _Nonnull strchrnul(const char* _Nonnull __s, int __ch) __attribute_pure__;
#endif
#endif

#if defined(__USE_GNU)
char* _Nonnull strerror_r(int __errno_value, char* _Nullable __buf, size_t __n) __RENAME(__gnu_strerror_r);
#endif

#if defined(__USE_GNU) && !defined(basename)
#if defined(__cplusplus)
extern "C++" char* _Nonnull basename(char* _Nullable __path) __RENAME(__gnu_basename);
extern "C++" const char* _Nonnull basename(const char* _Nonnull __path) __RENAME(__gnu_basename);
#else
char* _Nonnull basename(const char* _Nonnull __path) __RENAME(__gnu_basename);
#endif
#endif
__END_DECLS

#endif
