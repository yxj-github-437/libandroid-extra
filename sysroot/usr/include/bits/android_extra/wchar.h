#ifndef _WCHAR_H_
#error "Never include this file directly; instead, include <wchar.h>"
#endif

#ifdef __ANDROID_EXTRA

__BEGIN_DECLS
#if defined(__USE_GNU)
wchar_t* _Nonnull wmempcpy(wchar_t* _Nonnull __dst, const wchar_t* _Nonnull __src, size_t __n);
#endif

FILE* _Nullable open_wmemstream(wchar_t* _Nonnull * _Nonnull __ptr, size_t* _Nonnull  __size_ptr);
__END_DECLS

#endif
