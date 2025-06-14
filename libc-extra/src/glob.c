/*
 * Natanael Arndt, 2011: removed collate.h dependencies
 *  (my changes are trivial)
 *
 * Copyright (c) 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Guido van Rossum.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#define _GNU_SOURCE 1

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)glob.c	8.3 (Berkeley) 10/13/93";
#endif /* LIBC_SCCS and not lint */
#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

/*
 * glob(3) -- a superset of the one defined in POSIX 1003.2.
 *
 * The [!...] convention to negate a range is supported (SysV, Posix, ksh).
 *
 * Optional extra services, controlled by flags not defined by POSIX:
 *
 * GLOB_QUOTE:
 *	Escaping convention: \ inhibits any special meaning the following
 *	character might have (except \ at end of string is retained).
 * GLOB_MAGCHAR:
 *	Set in gl_flags if pattern contained a globbing character.
 * GLOB_NOMAGIC:
 *	Same as GLOB_NOCHECK, but it will only append pattern if it did
 *	not contain any magic characters.  [Used in csh style globbing]
 * GLOB_ALTDIRFUNC:
 *	Use alternately specified directory access functions.
 * GLOB_TILDE:
 *	expand ~user/foo to the /home/dir/of/user/foo
 * GLOB_BRACE:
 *	expand {1,2}{a,b} to 1a 1b 2a 2b
 * gl_matchc:
 *	Number of matches in the current invocation of glob.
 */

/*
 * Some notes on multibyte character support:
 * 1. Patterns with illegal byte sequences match nothing - even if
 *    GLOB_NOCHECK is specified.
 * 2. Illegal byte sequences in filenames are handled by treating them as
 *    single-byte characters with a value of the first byte of the sequence
 *    cast to wchar_t.
 * 3. State-dependent encodings are not currently supported.
 */

#include <sys/param.h>
#include <sys/stat.h>

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <glob.h>
#include <limits.h>
#include <pwd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>

#define DOLLAR '$'
#define DOT '.'
#define EOS '\0'
#define LBRACKET '['
#define NOT '!'
#define QUESTION '?'
#define QUOTE '\\'
#define RANGE '-'
#define RBRACKET ']'
#define SEP '/'
#define STAR '*'
#define TILDE '~'
#define UNDERSCORE '_'
#define LBRACE '{'
#define RBRACE '}'
#define SLASH '/'
#define COMMA ','

#ifndef DEBUG

#define M_QUOTE 0x8000000000ULL
#define M_PROTECT 0x4000000000ULL
#define M_MASK 0xffffffffffULL
#define M_CHAR 0x00ffffffffULL

typedef uint_fast64_t Char;

#else

#define M_QUOTE 0x80
#define M_PROTECT 0x40
#define M_MASK 0xff
#define M_CHAR 0x7f

typedef char Char;

#endif


#define CHAR(c) ((Char)((c)&M_CHAR))
#define META(c) ((Char)((c) | M_QUOTE))
#define M_ALL META('*')
#define M_END META(']')
#define M_NOT META('!')
#define M_ONE META('?')
#define M_RNG META('-')
#define M_SET META('[')
#define ismeta(c) (((c)&M_QUOTE) != 0)


static int compare(const void*, const void*);
static int g_Ctoc(const Char*, char*, size_t);
static int g_lstat(Char*, struct stat*, glob_t*);
static DIR* g_opendir(Char*, glob_t*);
static const Char* g_strchr(const Char*, wchar_t);
#ifdef notdef
static Char* g_strcat(Char*, const Char*);
#endif
static int g_stat(Char*, struct stat*, glob_t*);
static int glob0(const Char*, glob_t*, size_t*);
static int glob1(Char*, glob_t*, size_t*);
static int glob2(Char*, Char*, Char*, Char*, glob_t*, size_t*);
static int glob3(Char*, Char*, Char*, Char*, Char*, glob_t*, size_t*);
static int globextend(const Char*, glob_t*, size_t*);
static const Char* globtilde(const Char*, Char*, size_t, glob_t*);
static int globexp1(const Char*, glob_t*, size_t*);
static int globexp2(const Char*, const Char*, glob_t*, int*, size_t*);
static int match(Char*, Char*, Char*);
#ifdef DEBUG
static void qprintf(const char*, Char*);
#endif

int glob(const char* pattern, int flags, int (*errfunc)(const char*, int), glob_t* pglob)
{
    const char* patnext;
    size_t limit;
    Char *bufnext, *bufend, patbuf[MAXPATHLEN], prot;
    mbstate_t mbs;
    wchar_t wc;
    size_t clen;

    patnext = pattern;
    if (!(flags & GLOB_APPEND)) {
        pglob->gl_pathc = 0;
        pglob->gl_pathv = NULL;
        if (!(flags & GLOB_DOOFFS))
            pglob->gl_offs = 0;
    }
    if (flags & GLOB_LIMIT) {
        limit = pglob->gl_matchc;
        if (limit == 0)
            limit = sysconf(_SC_ARG_MAX);
    }
    else
        limit = 0;
    pglob->gl_flags = flags & ~GLOB_MAGCHAR;
    pglob->gl_errfunc = errfunc;
    pglob->gl_matchc = 0;

    bufnext = patbuf;
    bufend = bufnext + MAXPATHLEN - 1;
    if (flags & GLOB_NOESCAPE) {
        memset(&mbs, 0, sizeof(mbs));
        while (bufend - bufnext >= MB_CUR_MAX) {
            clen = mbrtowc(&wc, patnext, MB_LEN_MAX, &mbs);
            if (clen == (size_t)-1 || clen == (size_t)-2)
                return (GLOB_NOMATCH);
            else if (clen == 0)
                break;
            *bufnext++ = wc;
            patnext += clen;
        }
    }
    else {
        /* Protect the quoted characters. */
        memset(&mbs, 0, sizeof(mbs));
        while (bufend - bufnext >= MB_CUR_MAX) {
            if (*patnext == QUOTE) {
                if (*++patnext == EOS) {
                    *bufnext++ = QUOTE | M_PROTECT;
                    continue;
                }
                prot = M_PROTECT;
            }
            else
                prot = 0;
            clen = mbrtowc(&wc, patnext, MB_LEN_MAX, &mbs);
            if (clen == (size_t)-1 || clen == (size_t)-2)
                return (GLOB_NOMATCH);
            else if (clen == 0)
                break;
            *bufnext++ = wc | prot;
            patnext += clen;
        }
    }
    *bufnext = EOS;

    if (flags & GLOB_BRACE)
        return globexp1(patbuf, pglob, &limit);
    else
        return glob0(patbuf, pglob, &limit);
}

/*
 * Expand recursively a glob {} pattern. When there is no more expansion
 * invoke the standard globbing routine to glob the rest of the magic
 * characters
 */
static int globexp1(const Char* pattern, glob_t* pglob, size_t* limit)
{
    const Char* ptr = pattern;
    int rv;

    /* Protect a single {}, for find(1), like csh */
    if (pattern[0] == LBRACE && pattern[1] == RBRACE && pattern[2] == EOS)
        return glob0(pattern, pglob, limit);

    while ((ptr = g_strchr(ptr, LBRACE)) != NULL)
        if (!globexp2(ptr, pattern, pglob, &rv, limit))
            return rv;

    return glob0(pattern, pglob, limit);
}


/*
 * Recursive brace globbing helper. Tries to expand a single brace.
 * If it succeeds then it invokes globexp1 with the new pattern.
 * If it fails then it tries to glob the rest of the pattern and returns.
 */
static int globexp2(const Char* ptr, const Char* pattern, glob_t* pglob, int* rv, size_t* limit)
{
    int i;
    Char *lm, *ls;
    const Char *pe, *pm, *pm1, *pl;
    Char patbuf[MAXPATHLEN];

    /* copy part up to the brace */
    for (lm = patbuf, pm = pattern; pm != ptr; *lm++ = *pm++)
        continue;
    *lm = EOS;
    ls = lm;

    /* Find the balanced brace */
    for (i = 0, pe = ++ptr; *pe; pe++)
        if (*pe == LBRACKET) {
            /* Ignore everything between [] */
            for (pm = pe++; *pe != RBRACKET && *pe != EOS; pe++)
                continue;
            if (*pe == EOS) {
                /*
                 * We could not find a matching RBRACKET.
                 * Ignore and just look for RBRACE
                 */
                pe = pm;
            }
        }
        else if (*pe == LBRACE)
            i++;
        else if (*pe == RBRACE) {
            if (i == 0)
                break;
            i--;
        }

    /* Non matching braces; just glob the pattern */
    if (i != 0 || *pe == EOS) {
        *rv = glob0(patbuf, pglob, limit);
        return 0;
    }

    for (i = 0, pl = pm = ptr; pm <= pe; pm++)
        switch (*pm) {
            case LBRACKET:
                /* Ignore everything between [] */
                for (pm1 = pm++; *pm != RBRACKET && *pm != EOS; pm++)
                    continue;
                if (*pm == EOS) {
                    /*
                     * We could not find a matching RBRACKET.
                     * Ignore and just look for RBRACE
                     */
                    pm = pm1;
                }
                break;

            case LBRACE:
                i++;
                break;

            case RBRACE:
                if (i) {
                    i--;
                    break;
                }
                /* FALLTHROUGH */
            case COMMA:
                if (i && *pm == COMMA)
                    break;
                else {
                    /* Append the current string */
                    for (lm = ls; (pl < pm); *lm++ = *pl++)
                        continue;
                    /*
                     * Append the rest of the pattern after the
                     * closing brace
                     */
                    for (pl = pe + 1; (*lm++ = *pl++) != EOS;)
                        continue;

                        /* Expand the current pattern */
#ifdef DEBUG
                    qprintf("globexp2:", patbuf);
#endif
                    *rv = globexp1(patbuf, pglob, limit);

                    /* move after the comma, to the next string */
                    pl = pm + 1;
                }
                break;

            default:
                break;
        }
    *rv = 0;
    return 0;
}


/*
 * expand tilde from the passwd file.
 */
static const Char* globtilde(const Char* pattern, Char* patbuf, size_t patbuf_len, glob_t* pglob)
{
    struct passwd* pwd;
    char* h;
    const Char* p;
    Char *b, *eb;

    if (*pattern != TILDE || !(pglob->gl_flags & GLOB_TILDE))
        return pattern;

    /*
     * Copy up to the end of the string or /
     */
    eb = &patbuf[patbuf_len - 1];
    for (p = pattern + 1, h = (char*)patbuf; h < (char*)eb && *p && *p != SLASH; *h++ = *p++)
        continue;

    *h = EOS;

    if (((char*)patbuf)[0] == EOS) {
        /*
         * handle a plain ~ or ~/ by expanding $HOME first (iff
         * we're not running setuid or setgid) and then trying
         * the password file
         */
        if (
#ifndef __ANDROID__
            issetugid() != 0 ||
#endif
            (h = getenv("HOME")) == NULL) {
            if (((h = getlogin()) != NULL && (pwd = getpwnam(h)) != NULL)
                || (pwd = getpwuid(getuid())) != NULL)
                h = pwd->pw_dir;
            else
                return pattern;
        }
    }
    else {
        /*
         * Expand a ~user
         */
        if ((pwd = getpwnam((char*)patbuf)) == NULL)
            return pattern;
        else
            h = pwd->pw_dir;
    }

    /* Copy the home directory */
    for (b = patbuf; b < eb && *h; *b++ = *h++)
        continue;

    /* Append the rest of the pattern */
    while (b < eb && (*b++ = *p++) != EOS)
        continue;
    *b = EOS;

    return patbuf;
}


/*
 * The main glob() routine: compiles the pattern (optionally processing
 * quotes), calls glob1() to do the real pattern matching, and finally
 * sorts the list (unless unsorted operation is requested).  Returns 0
 * if things went well, nonzero if errors occurred.
 */
static int glob0(const Char* pattern, glob_t* pglob, size_t* limit)
{
    const Char* qpatnext;
    int err;
    size_t oldpathc;
    Char *bufnext, c, patbuf[MAXPATHLEN];

    qpatnext = globtilde(pattern, patbuf, MAXPATHLEN, pglob);
    oldpathc = pglob->gl_pathc;
    bufnext = patbuf;

    /* We don't need to check for buffer overflow any more. */
    while ((c = *qpatnext++) != EOS) {
        switch (c) {
            case LBRACKET:
                c = *qpatnext;
                if (c == NOT)
                    ++qpatnext;
                if (*qpatnext == EOS || g_strchr(qpatnext + 1, RBRACKET) == NULL) {
                    *bufnext++ = LBRACKET;
                    if (c == NOT)
                        --qpatnext;
                    break;
                }
                *bufnext++ = M_SET;
                if (c == NOT)
                    *bufnext++ = M_NOT;
                c = *qpatnext++;
                do {
                    *bufnext++ = CHAR(c);
                    if (*qpatnext == RANGE && (c = qpatnext[1]) != RBRACKET) {
                        *bufnext++ = M_RNG;
                        *bufnext++ = CHAR(c);
                        qpatnext += 2;
                    }
                } while ((c = *qpatnext++) != RBRACKET);
                pglob->gl_flags |= GLOB_MAGCHAR;
                *bufnext++ = M_END;
                break;
            case QUESTION:
                pglob->gl_flags |= GLOB_MAGCHAR;
                *bufnext++ = M_ONE;
                break;
            case STAR:
                pglob->gl_flags |= GLOB_MAGCHAR;
                /* collapse adjacent stars to one,
                 * to avoid exponential behavior
                 */
                if (bufnext == patbuf || bufnext[-1] != M_ALL)
                    *bufnext++ = M_ALL;
                break;
            default:
                *bufnext++ = CHAR(c);
                break;
        }
    }
    *bufnext = EOS;
#ifdef DEBUG
    qprintf("glob0:", patbuf);
#endif

    if ((err = glob1(patbuf, pglob, limit)) != 0)
        return (err);

    /*
     * If there was no match we are going to append the pattern
     * if GLOB_NOCHECK was specified or if GLOB_NOMAGIC was specified
     * and the pattern did not contain any magic characters
     * GLOB_NOMAGIC is there just for compatibility with csh.
     */
    if (pglob->gl_pathc == oldpathc) {
        if (((pglob->gl_flags & GLOB_NOCHECK)
             || ((pglob->gl_flags & GLOB_NOMAGIC) && !(pglob->gl_flags & GLOB_MAGCHAR))))
            return (globextend(pattern, pglob, limit));
        else
            return (GLOB_NOMATCH);
    }
    if (!(pglob->gl_flags & GLOB_NOSORT))
        qsort(pglob->gl_pathv + pglob->gl_offs + oldpathc,
              pglob->gl_pathc - oldpathc,
              sizeof(char*),
              compare);
    return (0);
}

static int compare(const void* p, const void* q)
{
    return (strcmp(*(char**)p, *(char**)q));
}

static int glob1(Char* pattern, glob_t* pglob, size_t* limit)
{
    Char pathbuf[MAXPATHLEN];

    /* A null pathname is invalid -- POSIX 1003.1 sect. 2.4. */
    if (*pattern == EOS)
        return (0);
    return (glob2(pathbuf, pathbuf, pathbuf + MAXPATHLEN - 1, pattern, pglob, limit));
}

/*
 * The functions glob2 and glob3 are mutually recursive; there is one level
 * of recursion for each segment in the pattern that contains one or more
 * meta characters.
 */
static int glob2(Char* pathbuf, Char* pathend, Char* pathend_last, Char* pattern, glob_t* pglob,
                 size_t* limit)
{
    struct stat sb;
    Char *p, *q;
    int anymeta;

    /*
     * Loop over pattern segments until end of pattern or until
     * segment with meta character found.
     */
    for (anymeta = 0;;) {
        if (*pattern == EOS) { /* End of pattern? */
            *pathend = EOS;
            if (g_lstat(pathbuf, &sb, pglob))
                return (0);

            if (((pglob->gl_flags & GLOB_MARK) && pathend[-1] != SEP)
                && (S_ISDIR(sb.st_mode)
                    || (S_ISLNK(sb.st_mode) && (g_stat(pathbuf, &sb, pglob) == 0)
                        && S_ISDIR(sb.st_mode)))) {
                if (pathend + 1 > pathend_last)
                    return (GLOB_ABORTED);
                *pathend++ = SEP;
                *pathend = EOS;
            }
            ++pglob->gl_matchc;
            return (globextend(pathbuf, pglob, limit));
        }

        /* Find end of next segment, copy tentatively to pathend. */
        q = pathend;
        p = pattern;
        while (*p != EOS && *p != SEP) {
            if (ismeta(*p))
                anymeta = 1;
            if (q + 1 > pathend_last)
                return (GLOB_ABORTED);
            *q++ = *p++;
        }

        if (!anymeta) { /* No expansion, do next segment. */
            pathend = q;
            pattern = p;
            while (*pattern == SEP) {
                if (pathend + 1 > pathend_last)
                    return (GLOB_ABORTED);
                *pathend++ = *pattern++;
            }
        }
        else /* Need expansion, recurse. */
            return (glob3(pathbuf, pathend, pathend_last, pattern, p, pglob, limit));
    }
    /* NOTREACHED */
}

static int glob3(Char* pathbuf, Char* pathend, Char* pathend_last, Char* pattern, Char* restpattern,
                 glob_t* pglob, size_t* limit)
{
    struct dirent* dp;
    DIR* dirp;
    int err;
    char buf[MAXPATHLEN];

    /*
     * The readdirfunc declaration can't be prototyped, because it is
     * assigned, below, to two functions which are prototyped in glob.h
     * and dirent.h as taking pointers to differently typed opaque
     * structures.
     */
    struct dirent* (*readdirfunc)();

    if (pathend > pathend_last)
        return (GLOB_ABORTED);
    *pathend = EOS;
    errno = 0;

    if ((dirp = g_opendir(pathbuf, pglob)) == NULL) {
        /* TODO: don't call for ENOENT or ENOTDIR? */
        if (pglob->gl_errfunc) {
            if (g_Ctoc(pathbuf, buf, sizeof(buf)))
                return (GLOB_ABORTED);
            if (pglob->gl_errfunc(buf, errno) || pglob->gl_flags & GLOB_ERR)
                return (GLOB_ABORTED);
        }
        return (0);
    }

    err = 0;

    /* Search directory for matching names. */
    if (pglob->gl_flags & GLOB_ALTDIRFUNC)
        readdirfunc = pglob->gl_readdir;
    else
        readdirfunc = readdir;
    while ((dp = (*readdirfunc)(dirp))) {
        char* sc;
        Char* dc;
        wchar_t wc;
        size_t clen;
        mbstate_t mbs;

        /* Initial DOT must be matched literally. */
        if (dp->d_name[0] == DOT && *pattern != DOT)
            continue;
        memset(&mbs, 0, sizeof(mbs));
        dc = pathend;
        sc = dp->d_name;
        while (dc < pathend_last) {
            clen = mbrtowc(&wc, sc, MB_LEN_MAX, &mbs);
            if (clen == (size_t)-1 || clen == (size_t)-2) {
                wc = *sc;
                clen = 1;
                memset(&mbs, 0, sizeof(mbs));
            }
            if ((*dc++ = wc) == EOS)
                break;
            sc += clen;
        }
        if (!match(pathend, pattern, restpattern)) {
            *pathend = EOS;
            continue;
        }
        err = glob2(pathbuf, --dc, pathend_last, restpattern, pglob, limit);
        if (err)
            break;
    }

    if (pglob->gl_flags & GLOB_ALTDIRFUNC)
        (*pglob->gl_closedir)(dirp);
    else
        closedir(dirp);
    return (err);
}


/*
 * Extend the gl_pathv member of a glob_t structure to accomodate a new item,
 * add the new item, and update gl_pathc.
 *
 * This assumes the BSD realloc, which only copies the block when its size
 * crosses a power-of-two boundary; for v7 realloc, this would cause quadratic
 * behavior.
 *
 * Return 0 if new item added, error code if memory couldn't be allocated.
 *
 * Invariant of the glob_t structure:
 *	Either gl_pathc is zero and gl_pathv is NULL; or gl_pathc > 0 and
 *	gl_pathv points to (gl_offs + gl_pathc + 1) items.
 */
static int globextend(const Char* path, glob_t* pglob, size_t* limit)
{
    char** pathv;
    size_t i, newsize, len;
    char* copy;
    const Char* p;

    if (*limit && pglob->gl_pathc > *limit) {
        errno = 0;
        return (GLOB_NOSPACE);
    }

    newsize = sizeof(*pathv) * (2 + pglob->gl_pathc + pglob->gl_offs);
    pathv = pglob->gl_pathv ? realloc((char*)pglob->gl_pathv, newsize) : malloc(newsize);
    if (pathv == NULL) {
        if (pglob->gl_pathv) {
            free(pglob->gl_pathv);
            pglob->gl_pathv = NULL;
        }
        return (GLOB_NOSPACE);
    }

    if (pglob->gl_pathv == NULL && pglob->gl_offs > 0) {
        /* first time around -- clear initial gl_offs items */
        pathv += pglob->gl_offs;
        for (i = pglob->gl_offs + 1; --i > 0;)
            *--pathv = NULL;
    }
    pglob->gl_pathv = pathv;

    for (p = path; *p++;)
        continue;
    len = MB_CUR_MAX * (size_t)(p - path); /* XXX overallocation */
    if ((copy = malloc(len)) != NULL) {
        if (g_Ctoc(path, copy, len)) {
            free(copy);
            return (GLOB_NOSPACE);
        }
        pathv[pglob->gl_offs + pglob->gl_pathc++] = copy;
    }
    pathv[pglob->gl_offs + pglob->gl_pathc] = NULL;
    return (copy == NULL ? GLOB_NOSPACE : 0);
}

/*
 * pattern matching function for filenames.  Each occurrence of the *
 * pattern causes a recursion level.
 */
static int match(Char* name, Char* pat, Char* patend)
{
    int ok, negate_range;
    Char c, k;

    while (pat < patend) {
        c = *pat++;
        switch (c & M_MASK) {
            case M_ALL:
                if (pat == patend)
                    return (1);
                do
                    if (match(name, pat, patend))
                        return (1);
                while (*name++ != EOS);
                return (0);
            case M_ONE:
                if (*name++ == EOS)
                    return (0);
                break;
            case M_SET:
                ok = 0;
                if ((k = *name++) == EOS)
                    return (0);
                if ((negate_range = ((*pat & M_MASK) == M_NOT)) != EOS)
                    ++pat;
                while (((c = *pat++) & M_MASK) != M_END)
                    if ((*pat & M_MASK) == M_RNG) {
                        if (CHAR(c) <= CHAR(k) && CHAR(k) <= CHAR(pat[1]))
                            ok = 1;
                        pat += 2;
                    }
                    else if (c == k)
                        ok = 1;
                if (ok == negate_range)
                    return (0);
                break;
            default:
                if (*name++ != c)
                    return (0);
                break;
        }
    }
    return (*name == EOS);
}

/* Free allocated data belonging to a glob_t structure. */
void globfree(glob_t* pglob)
{
    size_t i;
    char** pp;

    if (pglob->gl_pathv != NULL) {
        pp = pglob->gl_pathv + pglob->gl_offs;
        for (i = pglob->gl_pathc; i--; ++pp)
            if (*pp)
                free(*pp);
        free(pglob->gl_pathv);
        pglob->gl_pathv = NULL;
    }
}

static DIR* g_opendir(Char* str, glob_t* pglob)
{
    char buf[MAXPATHLEN];

    if (!*str)
        strcpy(buf, ".");
    else {
        if (g_Ctoc(str, buf, sizeof(buf)))
            return (NULL);
    }

    if (pglob->gl_flags & GLOB_ALTDIRFUNC)
        return ((*pglob->gl_opendir)(buf));

    return (opendir(buf));
}

static int g_lstat(Char* fn, struct stat* sb, glob_t* pglob)
{
    char buf[MAXPATHLEN];

    if (g_Ctoc(fn, buf, sizeof(buf))) {
        errno = ENAMETOOLONG;
        return (-1);
    }
    if (pglob->gl_flags & GLOB_ALTDIRFUNC)
        return ((*pglob->gl_lstat)(buf, sb));
    return (lstat(buf, sb));
}

static int g_stat(Char* fn, struct stat* sb, glob_t* pglob)
{
    char buf[MAXPATHLEN];

    if (g_Ctoc(fn, buf, sizeof(buf))) {
        errno = ENAMETOOLONG;
        return (-1);
    }
    if (pglob->gl_flags & GLOB_ALTDIRFUNC)
        return ((*pglob->gl_stat)(buf, sb));
    return (stat(buf, sb));
}

static const Char* g_strchr(const Char* str, wchar_t ch)
{

    do {
        if (*str == ch)
            return (str);
    } while (*str++);
    return (NULL);
}

static int g_Ctoc(const Char* str, char* buf, size_t len)
{
    mbstate_t mbs;
    size_t clen;

    memset(&mbs, 0, sizeof(mbs));
    while (len >= MB_CUR_MAX) {
        clen = wcrtomb(buf, *str, &mbs);
        if (clen == (size_t)-1)
            return (1);
        if (*str == L'\0')
            return (0);
        str++;
        buf += clen;
        len -= clen;
    }
    return (1);
}

#ifdef DEBUG
static void qprintf(const char* str, Char* s)
{
    Char* p;

    (void)printf("%s:\n", str);
    for (p = s; *p; p++)
        (void)printf("%c", CHAR(*p));
    (void)printf("\n");
    for (p = s; *p; p++)
        (void)printf("%c", *p & M_PROTECT ? '"' : ' ');
    (void)printf("\n");
    for (p = s; *p; p++)
        (void)printf("%c", ismeta(*p) ? '_' : ' ');
    (void)printf("\n");
}
#endif
