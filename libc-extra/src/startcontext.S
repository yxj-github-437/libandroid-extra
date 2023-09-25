#ifdef __ANDROID__
#if __aarch64__
# include "arm64-v8a/startcontext.S"
#elif __arm__
# include "armeabi-v7a/startcontext.S"
#elif __i386__
# include "x86/startcontext.S"
#elif __x86_64__
#include "x86_64/startcontext.S"
#endif

#else
# error "this file only support Android."
#endif
