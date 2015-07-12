#include <unistd.h> /* getopt() */
#include <stdlib.h> /* atoi(), exit(), EXIT_FAILURE */
#include <stdio.h>  /* fprintf(), printf() */
#include <float.h>  /* LDBL_DIG */

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

void calculateAndPrintPi(const double area) {
  printf("%.*f\n", LDBL_DIG, (4.0 * area));
}
