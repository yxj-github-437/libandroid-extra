#pragma once

#include <fcntl.h>

#ifdef __ANDROID_EXTRA

__BEGIN_DECLS
#if defined(__USE_GNU) || defined(__USE_BSD)
int lockf(int __fd, int __op, off_t __length) __RENAME_IF_FILE_OFFSET64(lockf64);
int lockf64(int __fd, int __op, off64_t __length);
#endif
__END_DECLS

#endif
