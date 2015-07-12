/**
 * HPCU Bi-Weekly Challenge: Distributed Ideal Gas
 * MPI version
 * Random - Defines a function for getting a random integer
 *
 * @author Aaron Weeden, Shodor Education Foundation
 * @version 3.0
 * @since 1.0
 */
#include <stdlib.h>
#include "random.h"

/**
 * Return a random integer between 0 (inclusive) and a maximum value
 * (exclusive).
 *
 * @param max the maximum value
 * @return the random integer
*/
int randomInt(int max) {

  return max * random() / RAND_MAX;

}
