/* Parallelization: Pi
 * By Oak Ridge National Laboratory
 * Source: https://www.olcf.ornl.gov/tutorials/monte-carlo-pi/
 * Modified by Phil List
 */

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <omp.h>
#include <math.h>
#include <time.h>
 
int main(int argc, char* argv[]) {

    int niter = 1000000;			// number of iterations per FOR loop
    int myid;					// holds process's rank id
    double x = 0, y = 0;			// x,y value for the random coordinate
    int i = 0;					// loop counter
    int count=0;				// Count holds all the number of how many good coordinates
    double z = 0;				// Used to check if x^2+y^2<=1
    double pi;					// holds approx value of pi
    int reducedcount;				// total number of "good" points from all nodes
    int reducedniter;				// total number of ALL points from all nodes
    int ranknum = 0;				// total number of nodes available
    int numthreads = 8;
    omp_set_num_threads(numthreads);
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &ranknum);
 
    if(myid != 0) {					// Do the following on all except the master node
        
        #pragma omp parallel firstprivate(x, y, z, i) reduction(+:count)
        {
            srandom((int)time(NULL) ^ omp_get_thread_num());    //Give random() a seed value
            for (i=0; i<niter; ++i) {
                x = (double)random()/RAND_MAX;      //gets a random x coordinate
                y = (double)random()/RAND_MAX;      //gets a random y coordinate
                z = sqrt((x*x)+(y*y));          //Checks to see if number is inside unit circle
                if (z<=1){
                    ++count;            //if it is, consider it a valid random point
                }
            }
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Reduce(&count, &reducedcount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    reducedniter = numthreads*niter*(ranknum-1);
 
    if (myid == 0) {                      // if root process/master node
        pi = ((double)reducedcount/(double)reducedniter)*4.0;
        printf("Pi: %f\n%i\n%d\n", pi, reducedcount, reducedniter);
    }
 
    MPI_Finalize();                     //Close the MPI instance
    return 0;
}

