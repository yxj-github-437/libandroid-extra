#ifndef _LANGINFO_H_
#error "Never include this file directly; instead, include <langinfo.h>"
#endif

#ifdef __ANDROID_EXTRA

__BEGIN_DECLS
char* _Nonnull nl_langinfo(nl_item __item);
char* _Nonnull nl_langinfo_l(nl_item __item, locale_t _Nonnull __l);
__END_DECLS

#endif
