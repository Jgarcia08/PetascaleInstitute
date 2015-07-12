/******************************************************************************
* FILE: pi-example.c
* DESCRIPTION:
*   Pi - OpenMP
*   Approximate pi using a Left Riemann Sum under a quarter unit circle.
*
*   When running the program, the number of rectangles can be passed using the
*   -r option, e.g. 'pi-example -r X', where X is the number of rectangles.
*
* ORIGINAL AUTHOR: Aaron Weeden
* MODIFIED BY: Phil List
******************************************************************************/

#include <omp.h>        /* to use OpenMP */
#include <stdio.h>      /* to use printf() */
#include <math.h>       /* to use sqrt() */
#include <stdlib.h>     /* to use malloc(), free() */
#include <unistd.h>     /* to use getopt() */

void getUserOptions(int argc, char **argv, int *numRects);

int main (int argc, char **argv) {
        int whichRect, numRects;
        double rectHeight, totalArea, leftXPos;

        numRects = 1000;          /* Each rectangle has a width of 1/numRects */
        totalArea = 0;
        getUserOptions(argc, argv, &numRects);
        double *areas = (double*)malloc(numRects * sizeof(double));

        #pragma omp parallel for private(rectHeight)

        for (whichRect=1; whichRect < numRects; whichRect++){

                //printf("thread num: %i of %i\n",omp_get_thread_num(), omp_get_num_threads());

                leftXPos = 1 / pow(whichRect, 4);       /* this rectangle's x-position */

               areas[whichRect-1] = leftXPos;/* a running total of the areas */
        }

        for(whichRect=1; whichRect < numRects; whichRect++){
                totalArea += areas[whichRect-1];
        }
        totalArea = sqrt(sqrt(totalArea*90));
        printf("Pi is about %f\n",totalArea);
        free(areas);
        return 0;
}

void getUserOptions(int argc, char **argv, int *numRects) {
  char c;

  while ((c = getopt(argc, argv, "r:")) != -1) {
    switch(c) {
      case 'r':
        (*numRects) = atoi(optarg);
        break;
      case '?':
      default:
        fprintf(stderr, "Usage: ");
        fprintf(stderr, "%s [-r numRects]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
  }
  argc -= optind;
  argv += optind;
}

