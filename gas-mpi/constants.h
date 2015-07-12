/**
 * HPCU Bi-Weekly Challenge: Distributed Ideal Gas
 * MPI version
 * Constants file - defines global constants.
 *
 * @author Aaron Weeden, Shodor Education Foundation
 * @version 3.0
 * @since 1.0
 */
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdbool.h>

/**
 * Indicates no particle exists in a particular flask cell.
 * Must be < 0.
 */
#define EMPTY -1

/* Directions */
#define UP 0
#define LEFT 1
#define DOWN 2
#define RIGHT 3
#define INTO_FLASK_1_STOPCOCK 4
#define INTO_FLASK_2_STOPCOCK 5

/**
 * Tag used for MPI calls.
 */
#define TAG 0

/* Flasks are defined as a 2D array; they have a certain number of rows and
 * columns defined below.  The stopcock for each flask is located at a certain
 * row in the flask, also defined below.
 */
static const int FLASK_1_ROW_COUNT = 24;
static const int FLASK_1_COLUMN_COUNT = 27;
static const int FLASK_1_STOPCOCK_ROW = 13;
static const int FLASK_2_ROW_COUNT = 69;
static const int FLASK_2_COLUMN_COUNT = 52;
static const int FLASK_2_STOPCOCK_ROW = 14;

/* Particles are located in a contiguous block in flask 1.  The dimensions and
 * location of that block are defined below.
 */
static const int PARTICLE_BLOCK_ROW_COUNT = 10;
static const int PARTICLE_BLOCK_COLUMN_COUNT = 12;
static const int PARTICLE_BLOCK_TOP_ROW = 7;
static const int PARTICLE_BLOCK_LEFT_COLUMN = 8;

/**
 * The simulation should run for this many time steps.
 */
static const int TIME_COUNT = 1000;

#endif
