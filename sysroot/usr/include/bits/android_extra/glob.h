#ifndef _GLOB_H_
#error "Never include this file directly; instead, include <glob.h>"
#endif

#ifdef __ANDROID_EXTRA

__BEGIN_DECLS
int glob(const char* _Nonnull __pattern, int __flags, int (* _Nullable __error_callback)(const char* _Nonnull __failure_path, int __failure_errno), glob_t* _Nonnull __result_ptr);
void globfree(glob_t* _Nonnull __result_ptr);
__END_DECLS

#endif
