#ifndef _STDC_PREDEF_H
#define _STDC_PREDEF_H  1

#ifndef __clang__
#ifdef __ANDROID__ && !defined(__ANDROID_API__)
# define __ANDROID_API__ __ANDROID_MIN_SDK_VERSION__
# define __ANDROID_MIN_SDK_VERSION__ 21
#endif

#define _Nonnull
#define _Nullable
#define _Null_unspecified
#endif

#define __ANDROID_EXTRA 1

#endif // _STDC_PREDEF_H
