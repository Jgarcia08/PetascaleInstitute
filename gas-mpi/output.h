/**
 * HPCU Bi-Weekly Challenge: Distributed Ideal Gas
 * MPI version
 * Output - Defines functions for drawing simulation data in ASCII.
 *
 * @author Aaron Weeden, Shodor Education Foundation
 * @version 3.0
 * @since 1.0
 */
#ifndef OUTPUT_H
#define OUTPUT_H

#include "typedefs.h"

/**
 * Prints an ASCII picture of the given time step.
 *
 * @param timeIdx ID of the current time step
 * @param flask1 the first flask
 * @param flask2 the second flask
 */
void printPicture(const int timeIdx, const flask_type flask1,
    const flask_type flask2);

/**
 * Calculates simulation properties related to number of particles and volume,
 * and prints them.  Also returns the ratio of two ratios: the number of
 * particles in flask 1 to the number of particles in flask 2, and the
 * volume of flask 1 to the volume of flask 2.
 *
 * @param particles list of particles
 * @param vratio ratio of volume of flask 1 to volume of flask 2
 * @return the ratio of ratios
 */
double calculateAndPrintSimulationProperties(particle_list_type particleList,
    const double vratio);

#endif
