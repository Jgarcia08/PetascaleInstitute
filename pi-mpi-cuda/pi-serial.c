/* Pi - serial version
 * Author: Aaron Weeden, Shodor, May 2015
 *
 * Approximate pi using a Left Riemann Sum under a quarter unit circle.
 *
 * When running the program, the number of rectangles can be passed using the
 * -r option, e.g. 'pi-serial -r X', where X is the number of rectangles.
 */

/*************
 * LIBRARIES *
 *************/
#include "pi-io.h"   /* getUserOptions(), calculateAndPrintPi() */
#include "pi-calc.h" /* calculateHeight() */

/*************************
 * FUNCTION DECLARATIONS *
 *************************/
void calculateArea(const int numRects, const double width, double *area);

/************************
 * FUNCTION DEFINITIONS *
 ************************/
int main(int argc, char **argv) {
  int numRects = 10;
  double area = 0.0;

  getUserOptions(argc, argv, &numRects);

  calculateArea(numRects, (1.0 / numRects), &area);

  calculateAndPrintPi(area);

  return 0;
}

void calculateArea(const int numRects, const double width, double *area) {
  int i = 0;
  double height = 0.0;

  (*area) = 0.0;
  for (i = 0; i < numRects; i++) {
    calculateHeight(i, width, &height);

    (*area) += (width * height);
  }
}
