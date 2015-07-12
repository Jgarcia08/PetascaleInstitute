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
#ifndef INITIALIZATION_C
#define INITIALIZATION_C

#include <stdlib.h>
#include "constants.h"
#include "memory-management.h"
#include "initialization.h"

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
    const int columnCount, const int stopcockRow) {

  int rowIdx, columnIdx;

  flask_p->id = id;
  flask_p->rowCount = rowCount;
  flask_p->columnCount = columnCount;
  flask_p->stopcockRow = stopcockRow;

  allocateFlaskCells(flask_p);

  for (rowIdx = 0; rowIdx < rowCount; rowIdx++) {
    for (columnIdx = 0; columnIdx < columnCount; columnIdx++) {
      flask_p->cells[rowIdx][columnIdx] = EMPTY;
    }
  }

  flask_p->stopcockCell = EMPTY;

}

/**
 * Allocate memory for particles and initialize their data.
 *
 * @param particleList_p pointer to the list of particles to initialize
 * @param flask_p pointer to the flask in which to place the particles.  This
 *   flask must have been previously initialized or an error may occur.
 * @param blockRowCount the number of rows in the initial block of particles
 * @param blockColumnCount the number of columns in the initial block particles
 */
void initializeParticles(particle_list_type *particleList_p,
    flask_type *flask_p, const int blockRowCount, const int blockColumnCount,
    const int topRow, const int leftColumn) {

  int particleIdx, rowIdx, columnIdx;

  particleList_p->count = blockRowCount * blockColumnCount;
  allocateParticles(particleList_p);

  particleIdx = 0;
  for (rowIdx = 0; rowIdx < blockRowCount; rowIdx++) {
    for (columnIdx = 0; columnIdx < blockColumnCount; columnIdx++) {
      flask_p->cells[topRow + rowIdx][leftColumn + columnIdx] = particleIdx;
      particleList_p->elements[particleIdx].id = particleIdx;
      particleList_p->elements[particleIdx].rowIdx = topRow + rowIdx;
      particleList_p->elements[particleIdx].columnIdx = leftColumn + columnIdx;
      particleList_p->elements[particleIdx].flask_p = flask_p;
      particleList_p->elements[particleIdx].isInStopcock = false;
      particleIdx++;
    }
  }

}

#endif
