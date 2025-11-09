#pragma once

#include <ifaddrs.h>

#ifdef __ANDROID_EXTRA

__BEGIN_DECLS
int getifaddrs(struct ifaddrs* _Nullable * _Nonnull __list_ptr);
void freeifaddrs(struct ifaddrs* _Nullable __ptr);
__END_DECLS

#endif
