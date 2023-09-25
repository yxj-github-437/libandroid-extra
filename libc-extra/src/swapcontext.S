#ifdef __ANDROID__
#if __aarch64__
# include "arm64-v8a/swapcontext.S"
#elif __arm__
# include "armeabi-v7a/swapcontext.S"
#elif __i386__
# include "x86/swapcontext.S"
#elif __x86_64__
#include "x86_64/swapcontext.S"
#endif

#else
# error "this file only support Android."
#endif
