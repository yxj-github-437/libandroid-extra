#pragma once

#include <complex.h>

#ifdef __ANDROID_EXTRA

__BEGIN_DECLS
double complex cacos(double complex __z);
float complex cacosf(float complex __z);

long double complex cacosl(long double complex __z);

double complex casin(double complex __z);
float complex casinf(float complex __z);

long double complex casinl(long double complex __z);

double complex catan(double complex __z);
float complex catanf(float complex __z);

long double complex catanl(long double complex __z);

double complex ccos(double complex __z);
float complex ccosf(float complex __z);

long double complex ccosl(long double complex __z);

double complex csin(double complex __z);
float complex csinf(float complex __z);

long double complex csinl(long double complex __z);

double complex ctan(double complex __z);
float complex ctanf(float complex __z);

long double complex ctanl(long double complex __z);

double complex cacosh(double complex __z);
float complex cacoshf(float complex __z);

long double complex cacoshl(long double complex __z);

double complex casinh(double complex __z);
float complex casinhf(float complex __z);

long double complex casinhl(long double complex __z);

double complex catanh(double complex __z);
float complex catanhf(float complex __z);

long double complex catanhl(long double complex __z);

double complex ccosh(double complex __z);
float complex ccoshf(float complex __z);

long double complex ccoshl(long double complex __z);

double complex csinh(double complex __z);
float complex csinhf(float complex __z);

long double complex csinhl(long double complex __z);

double complex ctanh(double complex __z);
float complex ctanhf(float complex __z);

long double complex ctanhl(long double complex __z);

double complex cexp(double complex __z);
float complex cexpf(float complex __z);

long double complex cexpl(long double complex __z);
/* 7.3.7.2 The clog functions */
double complex clog(double complex __z);
float complex clogf(float complex __z);
long double complex clogl(long double complex __z);

double cabs(double complex __z);
float cabsf(float complex __z);
long double cabsl(long double complex __z);

double complex cpow(double complex __x, double complex __z);
float complex cpowf(float complex __x, float complex __z);
long double complex cpowl(long double complex __x, long double complex __z);

double complex csqrt(double complex __z);
float complex csqrtf(float complex __z);
long double complex csqrtl(long double complex __z);

/* 7.3.9 Manipulation functions */
/* 7.3.9.1 The carg functions */
double carg(double complex __z);
float cargf(float complex __z);
long double cargl(long double complex __z);
/* 7.3.9.2 The cimag functions */
double cimag(double complex __z);
float cimagf(float complex __z);
long double cimagl(long double complex __z);
/* 7.3.9.3 The conj functions */
double complex conj(double complex __z);
float complex conjf(float complex __z);
long double complex conjl(long double complex __z);
/* 7.3.9.4 The cproj functions */
double complex cproj(double complex __z);
float complex cprojf(float complex __z);
long double complex cprojl(long double complex __z);
/* 7.3.9.5 The creal functions */
double creal(double complex __z);
float crealf(float complex __z);
long double creall(long double complex __z);
__END_DECLS

#endif
