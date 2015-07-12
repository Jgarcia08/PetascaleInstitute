/* Pi - CUDA version
 * Author: Aaron Weeden, Shodor, May 2015
 *
 * Approximate pi using a Left Riemann Sum under a quarter unit circle.
 *
 * When running the program, the number of rectangles can be passed using the
 * -r option, e.g. 'pi-cuda-1 -r X', where X is the number of rectangles.
 */

/*************
 * LIBRARIES *
 *************/
#include "pi-io.h" /* getUserOptions(), calculateAndPrintPi() */
#include "pi-calc-cuda.h" /* calculateArea() */

/************************
 * FUNCTION DEFINITIONS *
 ************************/
int main(int argc, char **argv) {
  int numRects = 10;
  double area = 0.0;

  getUserOptions(argc, argv, &numRects);

  calculateArea(numRects, &area);

  calculateAndPrintPi(area);

  return 0;
}
