#ifndef _LOCALCHARSET_H
#define _LOCALCHARSET_H

#include <langinfo.h>

__BEGIN_DECLS

static __inline __always_inline const char * locale_charset (void) {
    return nl_langinfo(CODESET);
}

__END_DECLS

#endif
