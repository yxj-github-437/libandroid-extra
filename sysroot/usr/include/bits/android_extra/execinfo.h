#ifndef _EXECINFO_H_
#error "Never include this file directly; instead, include <execinfo.h>"
#endif

#ifdef __ANDROID_EXTRA

__BEGIN_DECLS
int backtrace(void* _Nonnull * _Nonnull buffer, int size);
char* _Nullable * _Nullable backtrace_symbols(void* _Nonnull const* _Nonnull buffer, int size);
void backtrace_symbols_fd(void* _Nonnull const* _Nonnull buffer, int size, int fd);
__END_DECLS

#endif
