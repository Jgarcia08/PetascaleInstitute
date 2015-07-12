/**
 * HPCU Bi-Weekly Challenge: Distributed Ideal Gas
 * MPI version
 * Main file - runs a simulation of particles moving randomly in and between
 * two flasks connected by one stopcock per flask.
 *
 * @author Aaron Weeden, Shodor Education Foundation
 * @version 3.0
 * @since 1.0
 */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "constants.h"
#include "typedefs.h"
#include "initialization.h"
#include "simulation.h"
#include "memory-management.h"

/**
 * Runs the program.
 *
 * @param argc number of command-line arguments
 * @param argv array of command-line arguments
 * @return 0 on success, non-zero on failure
 */
int main(int argc, char **argv) {

  /* Declare constants whose values depend on other constants */
  const double V1 = FLASK_1_ROW_COUNT * FLASK_1_COLUMN_COUNT + 1.0;
  const double V2 = FLASK_2_ROW_COUNT * FLASK_2_COLUMN_COUNT + 1.0;
  const double VRATIO = V1 / V2;

  /* Declare model data */
  flask_type flask1, flask2;
  particle_list_type particleList;
  double ratio, sum;

  /* Declare MPI variables */
  int mpi_rank, mpi_size;

  /* Initialize MPI */
  MPI_Init(&argc, &argv);

  /* Set up MPI rank and size */
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

  /* Seed random number generator */
  srandom(time(NULL));

  /* Set up flasks and particles */
  initializeFlask(&flask1, 1, FLASK_1_ROW_COUNT, FLASK_1_COLUMN_COUNT,
    FLASK_1_STOPCOCK_ROW);
  initializeFlask(&flask2, 2, FLASK_2_ROW_COUNT, FLASK_2_COLUMN_COUNT,
    FLASK_2_STOPCOCK_ROW);
  initializeParticles(&particleList, &flask1, PARTICLE_BLOCK_ROW_COUNT,
    PARTICLE_BLOCK_COLUMN_COUNT, PARTICLE_BLOCK_TOP_ROW,
    PARTICLE_BLOCK_LEFT_COLUMN);

  /* Run the simulation */
  ratio = simulate(&flask1, &flask2, &particleList, TIME_COUNT, VRATIO);

  /* Rank 0 collects final ratios and sums them */
  MPI_Reduce(&ratio, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  /* Release allocated memory */
  free(particleList.elements);
  freeFlask(&flask2);
  freeFlask(&flask1);

  /* Rank 0 averages ratios and prints the result */
  if (mpi_rank == 0) {
    printf("Final ratio = %f\n", (sum/mpi_size));
  }

  /* Finalize MPI */
  MPI_Finalize();

  return 0;

}
