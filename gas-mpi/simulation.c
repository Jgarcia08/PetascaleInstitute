/**
 * HPCU Bi-Weekly Challenge: Distributed Ideal Gas
 * MPI version
 * Simulation file - defines the simulation function.
 *
 * @author Aaron Weeden, Shodor Education Foundation
 * @version 3.0
 * @since 1.0
 */
#include "typedefs.h"
#include "output.h"
#include "movement.h"
#include "simulation.h"

/**
 * Run a simulation of particles moving randomly in 2 flasks connected by
 * stopcocks for a given number of time steps.  Return a final ratio of two
 * ratios: the number of particles in flask 1 to the number of particles in
 *   flask 2, and the volume of flask 1 to the volume of flask 2.
 *
 * @param flask1_p pointer to the first flask, which must be previously
 *   initialized
 * @param flask2_p pointer to the second flask, which must be previously
 *   initialized
 * @param particleList_p pointer to the list of particles, which must be
 *   previously initialized
 * @param timeCount number of time steps for which to run the simulation
 * @param vratio ratio of volume of flask 1 to volume of flask 2
 * @return the final ratio
 */
double simulate(flask_type *flask1_p, flask_type *flask2_p,
    particle_list_type *particleList_p, const int timeCount,
    const double vratio) {

  int timeIdx;
  double ratio;

  for (timeIdx = 0; timeIdx < timeCount; timeIdx++) {
    /* Commented out for version 2.0 */
    /*printPicture(timeIdx, (*flask1_p), (*flask2_p));*/

    ratio = calculateAndPrintSimulationProperties((*particleList_p), vratio);

    moveParticlesRandomly(particleList_p, flask1_p, flask2_p);
  }

  return ratio;

}
