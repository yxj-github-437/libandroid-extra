#ifndef _IFADDRS_H_
#error "Never include this file directly; instead, include <ifaddrs.h>"
#endif

#ifdef __ANDROID_EXTRA

__BEGIN_DECLS
int getifaddrs(struct ifaddrs* _Nullable * _Nonnull __list_ptr);
void freeifaddrs(struct ifaddrs* _Nullable __ptr);
__END_DECLS

#endif
