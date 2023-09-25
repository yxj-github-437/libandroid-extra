#ifdef __ANDROID__
#if __aarch64__
# include "arm64-v8a/getcontext.S"
#elif __arm__
# include "armeabi-v7a/getcontext.S"
#elif __i386__
# include "x86/getcontext.S"
#elif __x86_64__
#include "x86_64/getcontext.S"
#endif

#else
# error "this file only support Android."
#endif
