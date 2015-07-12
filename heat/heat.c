/* PROGRAM: Simple Heat netCDF
 * AUTHORS: Aaron Weeden and Mobeen Ludin (Shodor Education Foundation)
 * DATE: January/February 2014
 *
 * See the README for instructions on compiling and running this program.
 */

#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <netcdf.h>

/* This is the name of the data file we will create. */
#define FILE_NAME "heat-data.nc"

/* We are writing 4D data, a 3 x 3 x 3 x-y-z grid, with 3 timesteps of data. */
#define NDIMS 4
#define NTIME 3
#define NX 3
#define NY 3
#define NZ 3
#define TIME_NAME "time"
#define X_NAME "columns"
#define Y_NAME "rows"
#define Z_NAME "depth"

/* Names of things. */
#define TEMP_NAME "temperature"
#define X_UNITS "cm"
#define Y_UNITS "cm"
#define Z_UNITS "cm"
#define TIME_UNITS "seconds"
#define TEMPERATURE_UNITS "degrees F"

/* For the units attributes. */
#define MAX_ATT_LEN 80

/* Handle errors by printing an error message and exiting in failure */
#define ERR(e) {printf("Error: %s\n", nc_strerror(e)); exit(EXIT_FAILURE);}

int main() {
    /* IDs for the netCDF file, dimensions, and variables. */
    int ncid, x_dimid, y_dimid, z_dimid, time_dimid;
    int x_varid, y_varid, z_varid, time_varid, temp_varid;
    int dimids[NDIMS];

    /* The start and count arrays will tell the netCDF library where to write
     * our data */
    size_t start[NDIMS], count[NDIMS];

    /* Program variables to hold the data we will write out. */
    float temp_out[NX][NY][NZ][NTIME];

    /* These program variables hold the x-coordinates, y-coordinates, and
     * time coordinates. */
    int time_coords[NTIME], x_coords[NX], y_coords[NY], z_coords[NZ];

    /* Loop indexes. */
    int time, x, y, z;

    /* Error handling. */
    int retval;

    /* Data Structure (what the model has) */
    float initialTemperature = 100.0;
    typedef struct {
        float temperatureNow;
        float temperatureNext;
        int neighborCount;
    } point_t;
    point_t points[NX][NY][NZ];

    /* Algorithm (how the model computes) */
    for (time = 0; time < NTIME; time++)
        time_coords[time] = time;
    for (x = 0; x < NX; x++)
        x_coords[x] = x;
    for (y = 0; y < NY; y++)
        y_coords[y] = y;
    for (z = 0; z < NZ; z++)
        z_coords[z] = z;
//*********************************************************
    /* Set initial temperature. */
    for (x = 0; x < NX; x++) {
        for (y = 0; y < NY; y++) {
              if (x == 0) {
                points[z][y][x].temperatureNow = initialTemperature;
            } else {
                points[z][y][x].temperatureNow = 0.0;
}}}



    for (x = 0; x < NX; x++) {
        for (y = 0; y < NY; y++) {
		 for (z = 0; z < NZ; z++) {
           /* if (x == 0) {
                points[z][y][x].temperatureNow = initialTemperature;
            } else {
                points[z][y][x].temperatureNow = 0.0;
            } */

            /* Assume 4 neighbors. */
            points[z][y][x].neighborCount = 4;

            /* Front column has 1 less neighbor. */
            if (z == 0)
                points[z][y][x].neighborCount--;

            /* Back column has 1 less neighbor. */
            if (z == NZ-1)
                points[z][y][x].neighborCount--;

            /* Top row has 1 less neighbor. */
            if (y == 0)
                points[z][y][x].neighborCount--;

            /* Bottom row has 1 less neighbor. */
            if (y == NY-1)
                points[z][y][x].neighborCount--;

            /* Left column has 1 less neighbor. */
            if (x == 0)
                points[z][y][x].neighborCount--;

            /* Right column has 1 less neighbor. */
            if (x == NX-1)
                points[z][y][x].neighborCount--;
		}
        }
    }

    /* Create the netCDF file. */
    if ((retval = nc_create(FILE_NAME, NC_CLOBBER, &ncid)))
        ERR(retval);

    /* Define the dimensions. */
    if ((retval = nc_def_dim(ncid, TIME_NAME, NTIME, &time_dimid)))
        ERR(retval);
    if ((retval = nc_def_dim(ncid, X_NAME, NX, &x_dimid)))
        ERR(retval);
    if ((retval = nc_def_dim(ncid, Y_NAME, NY, &y_dimid)))
        ERR(retval);
    if ((retval = nc_def_dim(ncid, Z_NAME, NY, &z_dimid)))
        ERR(retval);



    /* Define the coordinate variables. */
    if ((retval = nc_def_var(ncid, TIME_NAME, NC_FLOAT, 1, &time_dimid,
                    &time_varid)))
        ERR(retval);
    if ((retval = nc_def_var(ncid, X_NAME, NC_FLOAT, 1, &x_dimid, &x_varid)))
        ERR(retval);
    if ((retval = nc_def_var(ncid, Y_NAME, NC_FLOAT, 1, &y_dimid, &y_varid)))
        ERR(retval);
    if ((retval = nc_def_var(ncid, Z_NAME, NC_FLOAT, 1, &z_dimid, &z_varid)))
        ERR(retval);

    /* Assign units attributes to coordinate variables */
    if ((retval = nc_put_att_text(ncid, time_varid, TIME_UNITS, strlen(TIME_UNITS),
                    TIME_UNITS)))
        ERR(retval);
    if ((retval = nc_put_att_text(ncid, x_varid, X_UNITS, strlen(X_UNITS), X_UNITS)))
        ERR(retval);
    if ((retval = nc_put_att_text(ncid, y_varid, Y_UNITS, strlen(Y_UNITS), Y_UNITS)))
        ERR(retval);
    if ((retval = nc_put_att_text(ncid, z_varid, Z_UNITS, strlen(Z_UNITS), Z_UNITS)))
        ERR(retval);

    /* The dimids array is used to pass the dimids of the dimensions of
     * the netCDF variables. */
    dimids[0] = time_dimid;
    dimids[1] = x_dimid;
    dimids[2] = y_dimid;
    dimids[3] = z_dimid;

    /* Define the netCDF variables for the temperature data. */
    if ((retval = nc_def_var(ncid, TEMP_NAME, NC_FLOAT, NDIMS, dimids,
                    &temp_varid)))
        ERR(retval);

    /* Assign units attributes to the netCDF temperature variable. */
    if ((retval = nc_put_att_text(ncid, temp_varid, TEMPERATURE_UNITS, strlen(TEMPERATURE_UNITS),
                    TEMPERATURE_UNITS)))
        ERR(retval);

    /* End define mode */
    if ((retval = nc_enddef(ncid)))
        ERR(retval);

    /* Write the coordinate variable data. */
    if ((retval = nc_put_var_int(ncid, x_varid, &x_coords[0])))
        ERR(retval);
    if ((retval = nc_put_var_int(ncid, y_varid, &y_coords[0])))
        ERR(retval);
    if ((retval = nc_put_var_int(ncid, z_varid, &z_coords[0])))
        ERR(retval);
    if ((retval = nc_put_var_int(ncid, time_varid, &time_coords[0])))
        ERR(retval);

    /* These settings tell netCDF to write one timestep of data. */
    count[0] = NTIME;
    count[1] = NX;
    count[2] = NY;
    count[3] = NZ;
    start[0] = 0;
    start[1] = 0;
    start[2] = 0;
    start[3] = 0;
//*******************************************************
    for (time = 0; time < NTIME; time++) {
        for (x = 0; x < NX; x++) {
            for (y = 0; y < NY; y++) {
	            for (z = 0; z < NZ; z++) {

                temp_out[time][z][y][x] = points[z][y][x].temperatureNow;

                if ((retval = nc_put_vara_float(ncid, temp_varid, start, count,
                                &temp_out[0][0][0][0])))
                    ERR(retval);

                /* initialize next temperature. */
                points[z][y][x].temperatureNext = 0.0;

                /* add front neighbor. */
                if (z > 0) {
                    points[z][y][x].temperatureNext +=
                        points[z-1][y][x].temperatureNow;
                }

                /* add back neighbor. */
                if (z < NZ-1) {
                    points[z][y][x].temperatureNext +=
                        points[z+1][y][x].temperatureNow;
                }

                /* add top neighbor. */
                if (y > 0) {
                    points[z][y][x].temperatureNext +=
                        points[z][y-1][x].temperatureNow;
                }

                /* add bottom neighbor. */
                if (y < NY-1) {
                    points[z][y][x].temperatureNext +=
                        points[z][y+1][x].temperatureNow;
                }

                /* add left neighbor. */
                if (x > 0) {
                    points[z][y][x].temperatureNext +=
                        points[z][y][x-1].temperatureNow;
                }

                /* add right neighbor. */
                if (x < NX-1) {
                    points[z][y][x].temperatureNext +=
                        points[z][y][x+1].temperatureNow;
                }

                /* divide by number of neighbors. */
                points[z][y][x].temperatureNext =
                    points[z][y][x].temperatureNext /
                    points[z][y][x].neighborCount;
		}
            }
	}      
        for (x = 0; x < NX; x++) {
            for (y = 0; y < NY; y++) {
	            for (z = 0; z < NZ; z++) {

                /* advance temperature. */
                points[z][y][x].temperatureNow =
                    points[z][y][x].temperatureNext;
		}
            }
        }
    }

    /* Close the netCDF file. */
    if ((retval = nc_close(ncid)))
        ERR(retval);

    return 0;
}
