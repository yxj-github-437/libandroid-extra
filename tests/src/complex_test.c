#include <stdio.h>
#include <complex.h>
#include <math.h>

int main()
{
    double complex z = 1 + M_PI/3 * I;
    printf("z = %lf + %lfi\n", creal(z), cimag(z));
    
    double complex z_ = z * conj(z);
    printf("z_ = %lf + %lfi, arg(z_) = %lfπ\n",creal(z_), cimag(z_), carg(z_)/M_PI);
    
    z_ = clog(z_);
    printf("ln(z_) = %lf + %lfi\n", creal(z_), cimag(z_));
    
    double complex tmp = clog(z);
    printf("ln(z) = %lf + %lfi\n", creal(tmp), cimag(tmp));
    return 0;
}