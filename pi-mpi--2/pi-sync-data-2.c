#include <mpi.h> /* MPI_Reduce() */

void syncData(double *area) {
  double myArea = (*area);
  MPI_Reduce(&myArea, &(*area), 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
}
