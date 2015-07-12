/* Pi - MPI + CUDA version
 * Author: Aaron Weeden, Shodor, May 2015
 *
 * Approximate pi using a Left Riemann Sum under a quarter unit circle.
 *
 * When running the program, the number of rectangles can be passed using the
 * -r option, e.g. 'pi-mpi-cuda -r X', where X is the number of rectangles.
 */

/*************
 * LIBRARIES *
 *************/
#include <mpi.h> /* MPI_Finalize() */
#include "pi-io.h" /* getUserOptions(), calculateAndPrintPi() */
#include "pi-mpi.h" /* setupMPI(), distributeWork() */
#include "pi-mpi-cuda.h" /* calculateArea() */
#include "pi-sync-data-2.h" /* syncData() */

/************************
 * FUNCTION DEFINITIONS *
 ************************/
int main(int argc, char **argv) {
  int numRects = 10;
  double area = 0.0;
  int myRank = 0;
  int numProcs = 1;
  int myNumRects = 0;
  int myDispl = 0;

  setupMPI(&argc, &argv, &myRank, &numProcs);

  getUserOptions(argc, argv, &numRects);

  distributeWork(numRects, myRank, numProcs, &myNumRects, &myDispl);

  calculateArea(myNumRects, (1.0 / numRects), myDispl, &area);

  syncData(&area);

  if (myRank == 0) {
    calculateAndPrintPi(area);
  }

  MPI_Finalize();

  return 0;
}
