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
#ifndef MEMORY_MANAGEMENT_C
#define MEMORY_MANAGEMENT_C

#include <stdlib.h>
#include "typedefs.h"

/**
 * Allocate memory for a flask's cells.
 *
 * @param flask_p pointer to the flask whose cells' memory should be allocated
 */
void allocateFlaskCells(flask_type *flask_p) {

  int i;

  flask_p->cells = (int**)malloc(flask_p->rowCount * sizeof(int*));
  for (i = 0; i < flask_p->rowCount; i++) {
    flask_p->cells[i] = (int*)malloc(flask_p->columnCount * sizeof(int));
  }

}

/**
 * Allocate memory for a particle list.
 *
 * @param particleList_p pointer to the particle list for which to allocate
 *   memory
 */
void allocateParticles(particle_list_type *particleList_p) {

  particleList_p->elements = (particle_type*)malloc(particleList_p->count *
    sizeof(particle_type));

}

/**
 * Free memory for a flask's cells.
 *
 * @param flask_p pointer to the flask for which to free memory
 */
void freeFlask(flask_type *flask_p) {

  int i;

  for (i = flask_p->rowCount-1; i >= 0; i--) {
    free(flask_p->cells[i]);
  }
  free(flask_p->cells);

}

#endif
