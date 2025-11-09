#pragma once

#include <search.h>

#ifdef __ANDROID_EXTRA

__BEGIN_DECLS
int hcreate(size_t __n);
void hdestroy(void);
ENTRY* _Nullable hsearch(ENTRY __entry, ACTION __action);

#if defined(__USE_BSD) || defined(__USE_GNU)
int hcreate_r(size_t __n, struct hsearch_data* _Nonnull __table);
void hdestroy_r(struct hsearch_data* _Nonnull __table);
int hsearch_r(ENTRY __entry, ACTION __action, ENTRY* _Nullable * _Nonnull __result, struct hsearch_data* _Nonnull __table);
#endif

__END_DECLS

#endif
