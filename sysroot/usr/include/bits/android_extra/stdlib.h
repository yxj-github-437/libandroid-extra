#ifndef _STDLIB_H
#error "Never include this file directly; instead, include <stdlib.h>"
#endif

#ifdef __ANDROID_EXTRA

__BEGIN_DECLS
int mblen(const char* _Nullable __s, size_t __n);
__END_DECLS

#endif
