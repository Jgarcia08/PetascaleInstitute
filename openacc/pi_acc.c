/*
 * This is a simple program that shows the numerical integration example.
 * It computers pi by approximating the area under the curve:
 *      f(x) = 4 / (1+x*x) between 0 and 1.
 * To do this intergration numerically, the interval from 0 to 1 is divided into
 * some number (num_sub_intervals) subintervals and added up the area of rectangles
 * The larger the value of the num_sub_interval the more accurate your result well
 *
 * The program first asks the user to input a value for subintervals, it computes
 * the approximation for pi, and then compares it to a more accurate aproximate 
 * value of pi in math.h library.
 *
 * This program is just a serial version. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

#define PI_VALUE    3.14159265358979323846 // This value is copied from math.h

int main(int argc, char *argv[]) {
    long num_sub_intervals = 0;
    double start_time, end_time, time_diff;
    double x, pi=0.0;
    double sum = 0.0;

    printf("Please enter the number of iterations used to compute pi:\n ");
    scanf("%d",&num_sub_intervals);
    
    double h = 1.0/(double) num_sub_intervals;

    // Record the start time from here on:
    start_time = omp_get_wtime();
    
    int i;
    
//#pragma omp parallel for if(num_sub_intervals > 100) private(x) reduction(+:sum)
//#pragma acc data copy(x, num_sub_intervals), create(sum)
#pragma acc kernels copyin(x, num_sub_intervals), copyout(sum)
        for(i=0; i < num_sub_intervals; i++){
            x = (i+0.5)*h;
            sum += 4.0/(1.0+x*x);
        }
        pi = h *sum;


    //End recording the time here.
    end_time = omp_get_wtime();
    time_diff = end_time - start_time;

    // print the result here:
    printf("computed pi value is = %g (%17.15f)\n\n", pi,pi); 
    printf("PI accurate value from math.h is: %17.15f \n\n", PI_VALUE);
    printf("difference between computerd pi and math.h PI_VALUE = %17.15f\n\n", fabs(pi - PI_VALUE));
    printf("Time to computer = %g seconds\n\n", time_diff);

    return EXIT_SUCCESS;
}

