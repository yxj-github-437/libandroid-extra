#pragma once

#include <wctype.h>

#ifdef __ANDROID_EXTRA

__BEGIN_DECLS
wint_t towctrans(wint_t __wc, wctrans_t _Nonnull __transform);
wctrans_t _Nullable wctrans(const char* _Nonnull __name);

wint_t towctrans_l(wint_t __wc, wctrans_t _Nonnull __transform, locale_t _Nonnull __l);
wctrans_t _Nonnull wctrans_l(const char* _Nonnull __name, locale_t _Nonnull __l);
__END_DECLS

#endif
