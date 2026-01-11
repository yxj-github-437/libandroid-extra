#ifndef _SYS_MMAN_H_
#error "Never include this file directly; instead, include <sys/mman.h>"
#endif

#ifdef __ANDROID_EXTRA

__BEGIN_DECLS
int memfd_create(const char* _Nonnull __name, unsigned __flags);
__END_DECLS

#endif
