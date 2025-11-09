#pragma once

#include <sys/mman.h>

#ifdef __ANDROID_EXTRA

__BEGIN_DECLS
int memfd_create(const char* _Nonnull __name, unsigned __flags);
__END_DECLS

#endif
