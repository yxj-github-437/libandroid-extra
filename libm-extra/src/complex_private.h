#ifndef _MATH_PRIVATE_H_
#define _MATH_PRIVATE_H_

#include <complex.h>


#ifdef _COMPLEX_H

/*
 * C99 specifies that complex numbers have the same representation as
 * an array of two elements, where the first element is the real part
 * and the second element is the imaginary part.
 */
typedef union
{
    float complex z;
    float a[2];
} float_complex;
typedef union
{
    double complex z;
    double a[2];
} double_complex;
typedef union
{
    long double complex z;
    long double a[2];
} long_double_complex;
#define REALPART(z) ((z).a[0])
#define IMAGPART(z) ((z).a[1])

/*
 * Inline functions that can be used to construct complex values.
 *
 * The C99 standard intends x+I*y to be used for this, but x+I*y is
 * currently unusable in general since gcc introduces many overflow,
 * underflow, sign and efficiency bugs by rewriting I*y as
 * (0.0+I)*(y+0.0*I) and laboriously computing the full complex product.
 * In particular, I*Inf is corrupted to NaN+I*Inf, and I*-0 is corrupted
 * to -0.0+I*0.0.
 *
 * The C11 standard introduced the macros CMPLX(), CMPLXF() and CMPLXL()
 * to construct complex values.  Compilers that conform to the C99
 * standard require the following functions to avoid the above issues.
 */

#ifndef CMPLXF
static __inline float complex CMPLXF(float x, float y)
{
    float_complex w;

    REALPART(w) = x;
    IMAGPART(w) = y;
    return (w.z);
}
#endif

#ifndef CMPLX
static __inline double complex CMPLX(double x, double y)
{
    double_complex w;

    REALPART(w) = x;
    IMAGPART(w) = y;
    return (w.z);
}
#endif

#ifndef CMPLXL
static __inline long double complex CMPLXL(long double x, long double y)
{
    long_double_complex w;

    REALPART(w) = x;
    IMAGPART(w) = y;
    return (w.z);
}
#endif

#endif /* _COMPLEX_H */


#endif /* _MATH_PRIVATE_H_ */