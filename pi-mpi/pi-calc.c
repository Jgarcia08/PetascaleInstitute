#include <float.h> /* DBL_EPSILON and LDBL_DIG */
#include <math.h>  /* sqrt() */

void calculateHeight(const int i, const double width, double *height) {
  const double x = (i * width);
  const double heightSq = (1.0 - (x * x));

  /* Prevent nan value for sqrt() */
  (*height) = (heightSq < DBL_EPSILON) ? (0.0) : (sqrt(heightSq));
}
