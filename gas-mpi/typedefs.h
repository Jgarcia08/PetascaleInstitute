/**
 * HPCU Bi-Weekly Challenge: Distributed Ideal Gas
 * MPI version
 * Typedefs file - defines custom data types for the program.
 *
 * @author Aaron Weeden, Shodor Education Foundation
 * @version 3.0
 * @since 1.0
 */
#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <stdbool.h>

/**
 * Flask - contains row/column, stopcock, and contained particle information
 * about a rectangular flask.
 *
 * @since 1.0
 */
typedef struct {

  /**
   * Unique identifier for the flask.
   */
  int id;

  /**
   * Number of rows in the flask.
   */
  int rowCount;

  /**
   * Number of columns in the flask.
   */
  int columnCount;

  /**
   * Index of the row containing the stopcock, which connects the flask to
   * another flask.
   */
  int stopcockRow;

  /**
   * Each cell of the flask can contain a particle; this 2D array holds those
   * particles' unique identifiers, or EMPTY if there is no particle,
   * positioned in the array how the cells are positioned in the flask.
   */
  int **cells;

  /**
   * The stopcock can contain a particle; this holds that particle's unique
   * identifier or EMPTY if there is no particle.
   */
  int stopcockCell;

} flask_type;

/**
 * Particle - contains information about which flask the particle is in and
 * where it is in that flask.
 *
 * @since 1.0
 */
typedef struct {

  /**
   * Unique identifier for the particle.
   */
  int id;

  /**
   * Pointer to the flask containing the particle.
   */
  flask_type *flask_p;

  /**
   * The particle's row location within its containing flask.
   */
  int rowIdx;

  /**
   * The particle's column location within its containing flask.
   */
  int columnIdx;

  /**
   * true if the particle is within the stopcock of its containing flask, and
   * false otherwise.
   */
  bool isInStopcock;
} particle_type;

/**
 * Particle list - array of particles and its count of elements.
 *
 * @since 1.0
 */
typedef struct {
  /**
   * Array of particles.
   */
  particle_type *elements;

  /**
   * Number of particles in the array.
   */
  int count;

} particle_list_type;

/**
 * Open direction list - array of directions in which a particle can move and
 * its number of elements.
 *
 * @since 1.0
 */
typedef struct {

  /**
   * Array of open directions.
   */
  int elements[6];

  /**
   * Number of open directions in the array.
   */
  int count;

} open_direction_list_type;

#endif
