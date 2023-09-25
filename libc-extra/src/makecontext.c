#ifdef __ANDROID__
#if __aarch64__
# include "arm64-v8a/makecontext.c"
#elif __arm__
# include "armeabi-v7a/makecontext.c"
#elif __i386__
# include "x86/makecontext.c"
#elif __x86_64__
#include "x86_64/makecontext.c"
#endif

#else
# error "this file only support Android."
#endif