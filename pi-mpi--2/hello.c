#include <stdio.h>
#include <mpi.h>
#include <stdio.h>      /* to use printf() */
#include <math.h>       /* to use sqrt() */
#include <stdlib.h>     /* to use malloc(), free() */
#include <unistd.h>     /* to use getopt() */

int main(int argc, char **argv){
    int my_id;          // variable to save each process unique id in it

        int whichRect, numRects,temp;
        double totalArea, leftXPos;

        totalArea = 0;


    //Initializing MPI Library: parallel work starts from here on.
    MPI_Init(&argc, &argv);


    MPI_Comm_size(MPI_COMM_WORLD, &numRects);


    //Give Each process a unique integer id
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);


  if (my_id != 0) {

                leftXPos = 1 / pow(my_id, 4);      

               MPI_Send(&leftXPos, 1, MPI_DOUBLE, 0, 10, MPI_COMM_WORLD);
  }
  if (my_id == 0) { 

	
for(whichRect=1; whichRect < numRects; whichRect++){
printf("%d\n",whichRect);
	MPI_Recv(&temp, 1, MPI_DOUBLE, whichRect, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                totalArea += temp;
}
totalArea = sqrt(sqrt(totalArea*90));
        printf("Pi is about %f\n",totalArea);

}

    //Free up all the memory that was allocated to MPI Processes
    MPI_Finalize();

}
