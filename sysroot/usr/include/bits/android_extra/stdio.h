#pragma once

#include <stdio.h>

#ifdef __ANDROID_EXTRA

__BEGIN_DECLS
char* _Nonnull ctermid(char* _Nullable __buf);
__nodiscard FILE* _Nullable fmemopen(void* _Nullable __buf, size_t __size, const char* _Nonnull __mode);
__nodiscard FILE* _Nullable open_memstream(char* _Nonnull * _Nonnull __ptr, size_t* _Nonnull __size_ptr);

#if defined(__USE_BSD) || defined(__BIONIC__)
void clearerr_unlocked(FILE* _Nonnull __fp);
__nodiscard int feof_unlocked(FILE* _Nonnull __fp);
__nodiscard int ferror_unlocked(FILE* _Nonnull __fp);
__nodiscard int fileno_unlocked(FILE* _Nonnull __fp);
#endif
__END_DECLS

#endif
