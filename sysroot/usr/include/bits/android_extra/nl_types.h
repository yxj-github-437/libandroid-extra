#ifndef _NL_TYPES_H_
#error "Never include this file directly; instead, include <nl_types.h>"
#endif

#ifdef __ANDROID_EXTRA

__BEGIN_DECLS
nl_catd _Nonnull catopen(const char* _Nonnull __name, int __flag);
char* _Nonnull catgets(nl_catd _Nonnull __catalog, int __set_number, int __msg_number, const char* _Nonnull __msg);
int catclose(nl_catd _Nonnull __catalog);
__END_DECLS

#endif
