/**
 * HPCU Bi-Weekly Challenge: Distributed Ideal Gas
 * MPI version
 * Initialization - defines functions for setting up the initial flask and
 * particle data.
 *
 * @author Aaron Weeden, Shodor Education Foundation
 * @version 3.0
 * @since 1.0
 */
#ifndef INITIALIZATION_H
#define INITIALIZATION_H

#include "typedefs.h"

/**
 * Allocate memory for flask's cells and initialize its data.
 *
 * @param flask_p pointer to the flask to initialize
 * @param id the unique ID to assign to the flask
 * @param rowCount the number of rows to assign to the flask
 * @param columnCount the number of columns to assign to the flask
 * @param stopcockRow the row in which the flask's stopcock should be located
 */
void initializeFlask(flask_type *flask_p, const int id, const int rowCount,
    const int columnCount, const int stopcockRow);

/**
 * Allocate memory for particles and initialize their data.
 *
 * @param particleList_p pointer to the list of particles to initialize
 * @param flask pointer to the flask in which to place the particles.  This
 *   flask must have been previously initialized or an error may occur.
 * @param blockRowCount the number of rows in the initial block of particles
 * @param blockColumnCount the number of columns in the initial block particles
 */
void initializeParticles(particle_list_type *particleList_p,
    flask_type *flask_p, const int blockRowCount, const int blockColumnCount,
    const int topRow, const int leftColumn);

#endif
