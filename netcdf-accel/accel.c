/* 
 * HPCU Bi-Weekly Challenge: NetCDF Acceleration
 * Author: Aaron Weeden, Shodor, 2014
 * Last Modified: June 2, 2015, by Aaron Weeden
 */

/* import libraries */
#include <omp.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netcdf.h>

/* create macro for exiting if MPI errors occur */
#define tryMPI(ret, str, rank) \
    if (ret != MPI_SUCCESS) { \
        fprintf(stderr, "Rank %d returned error for %s, exiting!\n", \
                rank, str); \
        exit(EXIT_FAILURE); \
    }

/* This is the name of the data file we will create. */
#define FILE_NAME "accel-data.nc"

/* We are writing 1D data: pairs of initial position and final time data,
 * with (mpi_size * num_threads) total pairs. */
#define NDIMS 1

/* Names of things. */
#define INIT_POS_NAME "initial_position"
#define FINAL_TIME_NAME "final_time"
#define TEMP_NAME "temperature"
#define UNITS "units"

/* For the units attributes. */
#define MAX_ATT_LEN 80

/* Handle errors by printing an error message and exiting in failure */
#define ERR(e) { \
    printf("Error: %s\n", nc_strerror(e)); exit(EXIT_FAILURE); \
}

/* declare constants */
const double ACCELERATION = -9.81;
const double VELOCITY_0 = 0.0;
const double FACTOR = 1e6;
const int MPI_TAG = 0;

/* start program */
int main(int argc, char **argv) {

    /* declare variables */
    int mpi_rank;
    int mpi_size;
    int num_threads;
    double velocity;
    double position;
    int t;
    int i;
    int j;
    int *final_times;

    /* IDs for the netCDF file, dimensions, and variables. */
    int ncid, init_pos_dimid;
    int init_pos_varid, final_time_varid;
    int dimids[NDIMS];

    /* The start and count arrays will tell the netCDF library where to
     * write our data */
    size_t start[NDIMS], count[NDIMS];

     /* Program variables to hold the data we will write out. */
    int *final_time_out;

    /* This program variable holds the initial position coordinates. */
    double *init_pos_coords;

    /* Error handling. */
    int retval;

    /* set up mpi */
    tryMPI(
            MPI_Init(&argc, &argv),
            "MPI_Init",
            0
          );
    tryMPI(
            MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank),
            "MPI_Comm_rank",
            0
          );
    tryMPI(
            MPI_Comm_size(MPI_COMM_WORLD, &mpi_size),
            "MPI_Comm_size",
            mpi_rank
          );

    /* get number of threads */
#pragma omp parallel
    {
#pragma omp single
        {
            num_threads = omp_get_num_threads();
        }
    }

    /* allocate memory for array (enough space for each thread) */
    if ((final_times = malloc(num_threads * sizeof(int))) == NULL) {
        fprintf(stderr, "Rank %d returned error for malloc, exiting!\n",
                mpi_rank);
        exit(EXIT_FAILURE);
    }

    /* initialize velocity */
    velocity = VELOCITY_0;

    /* spawn OpenMP threads */
#pragma omp parallel private(position, velocity, t)
    {
        /* initialize position */
        position = FACTOR
            * (mpi_rank * num_threads + omp_get_thread_num());

        /* run the simulation */
        while (position > 0) {
            velocity += ACCELERATION;
            position += velocity;
            t++;
        }

        /* store the final time in an array */
        final_times[omp_get_thread_num()] = t;

        /* done spawning threads */
    }

    /* send array to rank 0 */
    tryMPI(
            MPI_Send(final_times, num_threads, MPI_INT, 0, MPI_TAG,
                MPI_COMM_WORLD),
            "MPI_Send",
            mpi_rank
          );

    /* rank 0 only */
    if (mpi_rank == 0) {

        /* Allocate memory for NetCDF arrays */
        if ((final_time_out = malloc(mpi_size * num_threads
                * sizeof(int))) == NULL) {
            fprintf(stderr,
                "Rank 0 returned error for final_time_out malloc, exiting!\n",
                mpi_rank);
            exit(EXIT_FAILURE);
        }
        if ((init_pos_coords = malloc(mpi_size * num_threads
                * sizeof(double))) == NULL) {
            fprintf(stderr,
                "Rank 0 returned error for init_pos_coords malloc, exiting!\n",
                mpi_rank);
            exit(EXIT_FAILURE);
        }

        /* Create the netCDF file. */
        if ((retval = nc_create(FILE_NAME, NC_CLOBBER, &ncid)))
            ERR(retval);

        /* Define the dimensions. */
        if ((retval = nc_def_dim(ncid, INIT_POS_NAME,
                mpi_size * num_threads, &init_pos_dimid)))
            ERR(retval);

        /* Define the coordinate variables. */
        if ((retval = nc_def_var(ncid, INIT_POS_NAME, NC_DOUBLE, 1,
                &init_pos_dimid, &init_pos_varid)))
            ERR(retval);

        /* Assign units attributes to coordinate variables */
        if ((retval = nc_put_att_text(ncid, init_pos_varid, UNITS,
                strlen(UNITS), UNITS)))
            ERR(retval);

        /* The dimids array is used to pass the dimids of the dimensions
         * of the netCDF variables. */
        dimids[0] = init_pos_dimid;

        /* Define the netCDF variable for the final time data. */
        if ((retval = nc_def_var(ncid, FINAL_TIME_NAME, NC_INT, NDIMS,
                dimids, &final_time_varid)))
            ERR(retval);

        /* Assign units attributes to the netCDF final time variable. */
        if ((retval = nc_put_att_text(ncid, final_time_varid, UNITS,
                strlen(UNITS), UNITS)))
            ERR(retval);

        /* End define mode */
        if ((retval = nc_enddef(ncid)))
            ERR(retval);

        /* Set up the coords variable */
        for (i = 0; i < mpi_size; i++) {
            for (j = 0; j < num_threads; j++) {
                init_pos_coords[i*num_threads+j] =
                    FACTOR * (i*num_threads+j);
            }
        }

        /* Write the coordinate variable data. */
        if ((retval = nc_put_var_double(ncid, init_pos_varid,
                &init_pos_coords[0])))
            ERR(retval);

        count[0] = mpi_size * num_threads;
        start[0] = 0;

        /* receive array from each process */
        for (i = 0; i < mpi_size; i++) {
            tryMPI(
                    MPI_Recv(final_times, num_threads, MPI_INT, i,
                        MPI_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE),
                    "MPI_Recv",
                    mpi_rank
                  );

            /* write out results */
            for (j = 0; j < num_threads; j++) {

                final_time_out[i*num_threads+j] =
                    final_times[j];

                if ((retval = nc_put_vara_int(ncid, final_time_varid,
                        start, count, &final_time_out[0])))
                    ERR(retval);

                printf("%f %d\n",
                        FACTOR * (i*num_threads+j),
                        final_times[j]);
            }
        }

        /* Close the netCDF file. */
        if ((retval = nc_close(ncid)))
            ERR(retval);

        /* Free memory for NetCDF arrays */
        free(init_pos_coords);
        free(final_time_out);
    }

    /* free memory for array */
    free(final_times);

    /* tear down mpi */
    tryMPI(
            MPI_Finalize(),
            "MPI_Finalize",
            mpi_rank
          );

    /* quit program */
    return 0;
}

