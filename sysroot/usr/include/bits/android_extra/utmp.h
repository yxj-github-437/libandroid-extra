#pragma once

#include <utmp.h>

#ifdef __ANDROID_EXTRA

__BEGIN_DECLS
int login_tty(int __fd);
__END_DECLS

#endif
