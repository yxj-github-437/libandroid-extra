#ifndef _PTY_H_
#error "Never include this file directly; instead, include <pty.h>"
#endif

#ifdef __ANDROID_EXTRA

__BEGIN_DECLS
int openpty(int* _Nonnull __pty_fd, int* _Nonnull __tty_fd, char* _Nullable __tty_name, const struct termios* _Nullable __termios_ptr, const struct winsize* _Nullable __winsize_ptr);
int forkpty(int* _Nonnull __parent_pty_fd, char* _Nullable __child_tty_name, const struct termios* _Nullable __termios_ptr, const struct winsize* _Nullable __winsize_ptr);
__END_DECLS

#endif
