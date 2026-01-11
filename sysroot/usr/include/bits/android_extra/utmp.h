#ifndef _UTMP_H_
#error "Never include this file directly; instead, include <utmp.h>"
#endif

#ifdef __ANDROID_EXTRA

__BEGIN_DECLS
int login_tty(int __fd);
__END_DECLS

#endif
