/* Copyright (C) 2015 by  Red Hat, Incorporated. All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software
 * is freely granted, provided that this notice is preserved.
 */

#include <complex.h>
#include "complex_private.h"

long double
creall (long double complex z)
{
  long_double_complex w = { .z = z };

  return (REALPART(w));
}
