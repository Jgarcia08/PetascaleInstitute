/**
 * HPCU Bi-Weekly Challenge: Distributed Ideal Gas
 * MPI version
 * Output - Defines functions for drawing simulation data in ASCII.
 *
 * @author Aaron Weeden, Shodor Education Foundation
 * @version 3.0
 * @since 1.0
 */
#include <stdio.h>
#include "constants.h"
#include "math.h"
#include "output.h"

/**
 * Prints an ASCII picture of the given time step.
 *
 * @param timeIdx ID of the current time step
 * @param flask1 the first flask
 * @param flask2 the second flask
 */
void printPicture(const int timeIdx, const flask_type flask1,
    const flask_type flask2) {

  int rowIdx, columnIdx;

  /* Print the current time step */
  printf("Time step: %d\n", timeIdx);

  /* Print top of flasks */
  printf("/");
  for (columnIdx = 0; columnIdx < flask1.columnCount; columnIdx++) {
    printf("-");
  }
  printf("+");
  for (columnIdx = 0; columnIdx < flask2.columnCount; columnIdx++) {
    printf("-");
  }
  printf("\\\n");

  /* Print flask rows up to bottom of flask 1 */
  for (rowIdx = 0; rowIdx < flask1.rowCount; rowIdx++) {
    /* Print flask 1 left wall */
    printf("|");

    /* Print empty space in flask 1 or particle */
    for (columnIdx = 0; columnIdx < flask1.columnCount; columnIdx++) {
      if (flask1.cells[rowIdx][columnIdx] == EMPTY) printf(" ");
      else printf("o");
    }

    /* Print empty flask 1 stopcock or particle */
    if (rowIdx == flask1.stopcockRow) {
      if (flask1.stopcockCell == EMPTY) printf(" ");
      else printf("o");
    }

    /* Print empty flask 2 stopcock or particle */
    else if (rowIdx == flask2.stopcockRow) {
      if (flask2.stopcockCell == EMPTY) printf(" ");
      else printf("o");
    }

    /* Print wall between flasks */
    else printf("|");

    /* Print empty space in flask 2 or particle */
    for (columnIdx = 0; columnIdx < flask2.columnCount; columnIdx++) {
      if (flask2.cells[rowIdx][columnIdx] == EMPTY) printf(" ");
      else printf("o");
    }

    /* Print right wall of flask 2 */
    printf("|\n");
  }

  /* Print bottom-left wall of flask 1 */
  printf("\\");

  /* Print flask rows below bottom of flask 1 */
  for (rowIdx = flask1.rowCount; rowIdx < flask2.rowCount; rowIdx++) {

    /* Print bottom wall of flask 1 or empty space to the left of flask 2 */
    if (rowIdx != flask1.rowCount) printf(" ");
    for (columnIdx = 0; columnIdx < flask1.columnCount; columnIdx++) {
      if (rowIdx == flask1.rowCount) printf("-");
      else printf(" ");
    }

    /* Print left wall of flask 2 */
    printf("|");

    /* Print particle or empty space in flask 2 */
    for (columnIdx = 0; columnIdx < flask2.columnCount; columnIdx++) {
      if (flask2.cells[rowIdx][columnIdx] == EMPTY) printf(" ");
      else printf("o");
    }

    /* Print right wall of flask 2 */
    printf("|\n");
  }

  /* Print empty space to the left of bottom wall of flask 2 */
  printf(" ");
  for (columnIdx = 0; columnIdx < flask1.columnCount; columnIdx++) {
    printf(" ");
  }

  /* Print bottom wall of flask 2 */
  printf("\\");
  for (columnIdx = 0; columnIdx < flask2.columnCount; columnIdx++) {
    printf("-");
  }
  printf("/\n");

}

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
    const double vratio) {

  int n1 = 0, n2 = 0;
  double nratio, ratio;
  int particleIdx;

  for (particleIdx = 0; particleIdx < particleList.count; particleIdx++) {
    if (particleList.elements[particleIdx].flask_p->id == 1) {
      n1++;
    }
    else {
      n2++;
    }
  }
  nratio = (n2 == 0) ? INFINITY : ((double)n1 / n2);
  ratio = (nratio == INFINITY) ? INFINITY : (nratio / vratio);

  /* Commented out for version 2.0 */
  /*printf("n1: %d\n", n1);
  printf("n2: %d\n", n2);
  printf("nratio: %f\n", nratio);
  printf("ratio: %f\n", ratio);
  printf("\n");*/

  return ratio;

}
