#include <limits.h>
#include <stdint.h>
#include <string.h>

#define ALIGN (sizeof(size_t))
#define ONES ((size_t)-1 / UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX / 2 + 1))
#define HASZERO(x) ((x)-ONES & ~(x)&HIGHS)

__BEGIN_DECLS

void* mempcpy(void* dest, const void* src, size_t n)
{
    return (char*)memcpy(dest, src, n) + n;
}

char* strchrnul(const char* s, int c)
{
    c = (unsigned char)c;
    if (!c)
        return (char*)s + strlen(s);

#ifdef __GNUC__
    typedef size_t __attribute__((__may_alias__)) word;
    const word* w;
    for (; (uintptr_t)s % ALIGN; s++)
        if (!*s || *(unsigned char*)s == c)
            return (char*)s;
    size_t k = ONES * c;
    for (w = (void*)s; !HASZERO(*w) && !HASZERO(*w ^ k); w++)
        ;
    s = (void*)w;
#endif
    for (; *s && *(unsigned char*)s != c; s++)
        ;
    return (char*)s;
}

char* strerror_l(int error, locale_t l)
{
    return strerror(error);
}


const char* __gnu_basename(const char* path)
{
    const char* last_slash = strrchr(path, '/');
    return (last_slash != NULL) ? last_slash + 1 : path;
}

__END_DECLS
