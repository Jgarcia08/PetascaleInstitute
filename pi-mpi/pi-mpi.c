#include <mpi.h>     /* MPI_Init(), MPI_Comm_rank(), etc. */
#include "pi-calc.h" /* calculateHeight() */

void setupMPI(int *argc, char ***argv, int *myRank, int *numProcs) {
  MPI_Init(&(*argc), &(*argv));
  MPI_Comm_rank(MPI_COMM_WORLD, &(*myRank));
  MPI_Comm_size(MPI_COMM_WORLD, &(*numProcs));
}

/* Split rectangles as evenly as possible, give each of the first N processes
 * 1 of the remaining rectangles */
void distributeWork(const int numRects, const int myRank, const int numProcs,
    int *myNumRects, int *myDispl) {
  const int evenSplit = (numRects / numProcs);
  const int numProcsWith1Extra = (numRects % numProcs);

  if (myRank < numProcsWith1Extra) {
    (*myNumRects) = (evenSplit + 1);
    (*myDispl) = (myRank * (evenSplit + 1));
  }
  else {
    (*myNumRects) = evenSplit;
    (*myDispl) = (numProcsWith1Extra * (evenSplit + 1)) +
      ((myRank - numProcsWith1Extra) * evenSplit);
  }
}

void calculateArea(const int numRects, const int myNumRects, const double width,
    const int myDispl, double *area) {
  int i = 0;
  double height = 0.0;

  for (i = myDispl; i < (myDispl + myNumRects); i++) {
    calculateHeight(i, width, &height);

    (*area) += (width * height);
  }
}
