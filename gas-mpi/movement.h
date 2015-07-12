/**
 * HPCU Bi-Weekly Challenge: Distributed Ideal Gas
 * MPI version
 * Movement - Defines functions for moving particles randomly
 *
 * @author Aaron Weeden, Shodor Education Foundation
 * @version 3.0
 * @since 1.0
 */
#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "typedefs.h"

/**
 * Given a particle, calculate which directions it can move.
 *
 * @param particle the particle
 * @param flask1 the first flask
 * @param flask2 the second flask
 * @return list of directions in which the particle can move
 */
open_direction_list_type calculateOpenDirections(const particle_type particle,
  const flask_type flask1, const flask_type flask2);

/**
 * Given a particle, removes it from its containing flask.
 *
 * @param particle_p pointer to the particle to remove
 */
void removeParticleFromFlaskCell(particle_type *particle_p);

/**
 * Given a particle, moves it in a given direction.
 *
 * @param particle_p pointer to the particle to move
 * @param direction the direction for the particle to move
 * @param flask1_p pointer to the first flask
 * @param flask2_p pointer to the second flask
 */
void moveParticleInDirection(particle_type *particle_p, const int direction,
    flask_type * const flask1_p, flask_type * const flask2_p);

/**
 * Moves all particles in a list randomly.  Each particle moves at most 1
 * square in a random direction.
 *
 * @param particleList_p pointer to the list of particles to move randomly
 * @param flask1_p pointer to the first flask
 * @param flask2_p pointer to the second flask
 */
void moveParticlesRandomly(particle_list_type *particleList_p,
   flask_type *flask1_p, flask_type *flask2_p);

#endif
