#include <mpi.h> /* MPI_Recv(), MPI_Send(), etc. */

#define TAG 0

void syncData(const int myRank, const int numProcs, double *myArea) {
  int i;
  double theirArea;

  if (myRank == 0) {
    for (i = 1; i < numProcs; i++) {
      MPI_Recv(&theirArea, 1, MPI_DOUBLE, i, TAG, MPI_COMM_WORLD,
        MPI_STATUS_IGNORE);
      (*myArea) += theirArea;
    }
  }
  else {
    MPI_Send(&(*myArea), 1, MPI_DOUBLE, TAG, 0, MPI_COMM_WORLD);
  }
}
