/* Pi - MPI version 1 (uses MPI_Send() and MPI_Recv())
 * Author: Aaron Weeden, Shodor, May 2015
 *
 * Approximate pi using a Left Riemann Sum under a quarter unit circle.
 *
 * When running the program, the number of rectangles can be passed using the
 * -r option, e.g. 'pi-mpi-1 -r X', where X is the number of rectangles.
 */

/*************
 * LIBRARIES *
 *************/
#include <mpi.h> /* MPI_Send(), MPI_Recv(), etc. */
#include "pi-io.h" /* getUserOptions(), calculateAndPrintPi() */
#include "pi-mpi.h" /* setupMPI(), distributeWork(), calculateArea() */
#include "pi-sync-data-1.h" /* syncData() */

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

  calculateArea(numRects, myNumRects, (1.0 / numRects), myDispl, &area);

  syncData(myRank, numProcs, &area);

  if (myRank == 0) {
    calculateAndPrintPi(area);
  }

  MPI_Finalize();

  return 0;
}
