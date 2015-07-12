/**
 * HPCU Bi-Weekly Challenge: Distributed Ideal Gas
 * MPI version
 * Memory management - defines functions for allocating and freeing memory for
 * flask and particle data.
 *
 * @author Aaron Weeden, Shodor Education Foundation
 * @version 3.0
 * @since 1.0
 */
#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

#include "typedefs.h"

/**
 * Allocate memory for a flask's cells.
 *
 * @param flask_p pointer to the flask whose cells' memory should be allocated
 */
void allocateFlaskCells(flask_type *flask_p);

/**
 * Allocate memory for a particle list.
 *
 * @param particleList_p pointer to the particle list for which to allocate
 *   memory
 */
void allocateParticles(particle_list_type *particleList_p);

/**
 * Free memory for a flask.
 *
 * @param flask_p pointer to the flask for which to free memory
 */
void freeFlask(flask_type *flask_p);

#endif
