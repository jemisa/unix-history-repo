#include "f2c.h"

#undef abs
#include <math.h>
double
d_sinh (doublereal * x)
{
  return (sinh (*x));
}
