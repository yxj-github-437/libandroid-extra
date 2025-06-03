#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <wctype.h>

#define wctrans_tolower (wctrans_t)1
#define wctrans_toupper (wctrans_t)2

wint_t towctrans(wint_t c, wctrans_t t) {
    if (t == wctrans_tolower) return towlower(c);
    if (t == wctrans_toupper) return towupper(c);
    errno = EINVAL;
    return 0;
}

wctrans_t wctrans(const char* name) {
    if (strcmp(name, "tolower") == 0) return wctrans_tolower;
    if (strcmp(name, "toupper") == 0) return wctrans_toupper;
    return 0;
}

wint_t towctrans_l(wint_t c, wctrans_t t, locale_t) {
    return towctrans(c, t);
}

wctrans_t wctrans_l(const char* name, locale_t) {
    return wctrans(name);
}
