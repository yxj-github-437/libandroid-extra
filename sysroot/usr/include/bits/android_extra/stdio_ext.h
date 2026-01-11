#ifndef _STDIO_EXT_H_
#error "Never include this file directly; instead, include <stdio_ext.h>"
#endif

#ifdef __ANDROID_EXTRA

__BEGIN_DECLS
size_t __fbufsize(FILE* _Nonnull __fp);
int __freadable(FILE* _Nonnull __fp);
int __freading(FILE* _Nonnull __fp);
int __fwritable(FILE* _Nonnull __fp);
int __fwriting(FILE* _Nonnull __fp);
int __flbf(FILE* _Nonnull __fp);
size_t __fpending(FILE* _Nonnull __fp);
size_t __freadahead(FILE* _Nonnull __fp);
void _flushlbf(void);
void __fseterr(FILE* _Nonnull __fp);
int __fsetlocking(FILE* _Nonnull __fp, int __type);
__END_DECLS


#endif
