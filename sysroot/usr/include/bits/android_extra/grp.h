#pragma once

#include <grp.h>

#ifdef __ANDROID_EXTRA

__BEGIN_DECLS
struct group* _Nullable getgrent(void);

void setgrent(void);
void endgrent(void);

int getgrgid_r(gid_t __gid, struct group* __BIONIC_COMPLICATED_NULLNESS __group, char* _Nonnull __buf, size_t __n, struct group* _Nullable * _Nonnull __result);
int getgrnam_r(const char* _Nonnull __name, struct group* __BIONIC_COMPLICATED_NULLNESS __group, char* _Nonnull __buf, size_t __n, struct group* _Nullable *_Nonnull __result);
__END_DECLS

#endif
