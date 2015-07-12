/**
 * HPCU Bi-Weekly Challenge: Distributed Ideal Gas
 * MPI version
 * Movement - Defines functions for moving particles randomly
 *
 * @author Aaron Weeden, Shodor Education Foundation
 * @version 3.0
 * @since 1.0
 */
#include "constants.h"
#include "random.h"
#include "movement.h"

/**
 * Given a particle, calculate which directions it can move.
 *
 * @param particle the particle
 * @param flask1 the first flask
 * @param flask2 the second flask
 * @return list of directions in which the particle can move
 */
open_direction_list_type calculateOpenDirections(const particle_type particle,
    const flask_type flask1, const flask_type flask2) {

  open_direction_list_type openDirectionList;

  openDirectionList.count = 0;
  if (particle.isInStopcock) {
    if (1 == particle.flask_p->id) {
      if (flask2.stopcockCell == EMPTY) {
        openDirectionList.elements[openDirectionList.count++] =
          INTO_FLASK_2_STOPCOCK;
      }
      if (flask1.cells[flask1.stopcockRow][flask1.columnCount-1] == EMPTY) {
        openDirectionList.elements[openDirectionList.count++] = LEFT;
      }
      if (flask2.cells[flask1.stopcockRow][0] == EMPTY) {
        openDirectionList.elements[openDirectionList.count++] = RIGHT;
      }
    }
    else {
      if (flask1.stopcockCell == EMPTY) {
        openDirectionList.elements[openDirectionList.count++] =
          INTO_FLASK_1_STOPCOCK;
      }
      if (flask1.cells[flask2.stopcockRow][flask1.columnCount-1] == EMPTY) {
        openDirectionList.elements[openDirectionList.count++] = LEFT;
      }
      if (flask2.cells[flask2.stopcockRow][0] == EMPTY) {
        openDirectionList.elements[openDirectionList.count++] = RIGHT;
      }
    }
  }
  else {
    if (particle.rowIdx > 0 &&
        particle.flask_p->cells[particle.rowIdx-1][particle.columnIdx] == EMPTY) {
      openDirectionList.elements[openDirectionList.count++] = UP;
    }
    if (particle.columnIdx > 0 &&
        particle.flask_p->cells[particle.rowIdx][particle.columnIdx-1] == EMPTY) {
      openDirectionList.elements[openDirectionList.count++] = LEFT;
    }
    if (particle.rowIdx < particle.flask_p->rowCount-1 &&
        particle.flask_p->cells[particle.rowIdx+1][particle.columnIdx] == EMPTY) {
      openDirectionList.elements[openDirectionList.count++] = DOWN;
    }
    if (particle.columnIdx < particle.flask_p->columnCount-1 &&
        particle.flask_p->cells[particle.rowIdx][particle.columnIdx+1] == EMPTY) {
      openDirectionList.elements[openDirectionList.count++] = RIGHT;
    }
    if ((1 == particle.flask_p->id &&
          particle.columnIdx == flask1.columnCount-1) ||
        (2 == particle.flask_p->id &&
         particle.columnIdx == 0)) {
      if (flask1.stopcockCell == EMPTY &&
          particle.rowIdx == flask1.stopcockRow) {
        openDirectionList.elements[openDirectionList.count++] =
          INTO_FLASK_1_STOPCOCK;
      }
      if (flask2.stopcockCell == EMPTY &&
          flask2.stopcockRow == particle.rowIdx) {
        openDirectionList.elements[openDirectionList.count++] =
          INTO_FLASK_2_STOPCOCK;
      }
    }
  }

  return openDirectionList;

}

/**
 * Given a particle, removes it from its containing flask.
 *
 * @param particle_p pointer the particle to remove
 */
void removeParticleFromFlaskCell(particle_type *particle_p) {

  if (particle_p->isInStopcock) {
    particle_p->flask_p->stopcockCell = EMPTY;
  }
  else {
    particle_p->flask_p->cells[particle_p->rowIdx][particle_p->columnIdx] =
      EMPTY;
  }

}

/**
 * Given a particle, moves it in a given direction.
 *
 * @param particle_p pointer to the particle to move
 * @param direction the direction for the particle to move
 * @param flask1_p pointer to the first flask
 * @param flask2_p pointer to the second flask
 */
void moveParticleInDirection(particle_type *particle_p, const int direction,
    flask_type * const flask1_p, flask_type * const flask2_p) {

  const int rowIdx = particle_p->rowIdx;
  int columnIdx = particle_p->columnIdx;
  const int id = particle_p->id;
  const bool isInStopcock = particle_p->isInStopcock;

  removeParticleFromFlaskCell(particle_p);

  switch(direction) {
    case UP:
      particle_p->flask_p->cells[rowIdx-1][columnIdx] = id;
      particle_p->rowIdx = rowIdx-1;
      break;
    case LEFT:
      if (isInStopcock) {
        particle_p->flask_p = flask1_p;
        columnIdx = flask1_p->columnCount;
      }
      particle_p->flask_p->cells[rowIdx][columnIdx-1] = id;
      particle_p->columnIdx = columnIdx-1;
      particle_p->isInStopcock = false;
      break;
    case DOWN:
      particle_p->flask_p->cells[rowIdx+1][columnIdx] = id;
      particle_p->rowIdx = rowIdx+1;
      break;
    case RIGHT:
      if (isInStopcock) {
        particle_p->flask_p = flask2_p;
        columnIdx = -1;
      }
      particle_p->flask_p->cells[rowIdx][columnIdx+1] = id;
      particle_p->columnIdx = columnIdx+1;
      particle_p->isInStopcock = false;
      break;
    case INTO_FLASK_1_STOPCOCK:
    case INTO_FLASK_2_STOPCOCK:
      if (INTO_FLASK_1_STOPCOCK == direction) {
        particle_p->flask_p = flask1_p;
      }
      else {
        particle_p->flask_p = flask2_p;
      }
      particle_p->flask_p->stopcockCell = id;
      particle_p->isInStopcock = true;
      break;
    }

}

/**
 * Moves all particles in a list randomly.  Each particle moves at most 1
 * square in a random direction.
 *
 * @param particleList_p pointer to the list of particles to move randomly
 * @param flask1_p pointer to the first flask
 * @param flask2_p pointer to the second flask
 */
void moveParticlesRandomly(particle_list_type *particleList_p,
   flask_type *flask1_p, flask_type *flask2_p) {

  particle_type *particle_p;
  int particleIdx;
  open_direction_list_type openDirectionList;

  for (particleIdx = 0; particleIdx < particleList_p->count; particleIdx++) {
    particle_p = &(particleList_p->elements[particleIdx]);

    openDirectionList = calculateOpenDirections((*particle_p),
      (*flask1_p), (*flask2_p));

    if (openDirectionList.count > 0) {
      moveParticleInDirection(particle_p,
        openDirectionList.elements[randomInt(openDirectionList.count)],
        flask1_p, flask2_p);
    }
  }

}
