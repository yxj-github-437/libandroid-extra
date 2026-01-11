#ifndef _UNISTD_H_
#error "Never include this file directly; instead, include <unistd.h>"
#endif

#ifdef __ANDROID_EXTRA

__BEGIN_DECLS
char *getpass(const char *prompt);

int lockf(int fd, int op, off_t size) __RENAME_IF_FILE_OFFSET64(lockf64);
int lockf64(int __fd, int __op, off64_t __length);
__END_DECLS

#endif
