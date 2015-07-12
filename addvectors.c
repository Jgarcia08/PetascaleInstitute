#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
int main( int argc, char* argv[] )
{
 
    // Size of vectors
    int n = 10000;
 
    // Input vectors
    double *restrict a;
    double *restrict b;
    // Output vector
    double *restrict c;
    double sum = 0.0;
    
    // Size, in bytes, of each vector
    size_t bytes = n*sizeof(double);
 
    // Allocate memory for each vector
    a = (double*)malloc(bytes);
    b = (double*)malloc(bytes);
    c = (double*)malloc(bytes);
 
    // Initialize content of input vectors, vector a[i] = sin(i)^2 vector b[i] = cos(i)^2
    int i;
    for(i=0; i<n; i++) {
        a[i] = sin(i)*sin(i);
        b[i] = cos(i)*cos(i);
    }   
#pragma acc data copyin(a[0:n],b[0:n]) copy(c[0:n]) copy(sum)
    {   // sum component wise and save result into vector c
    #pragma acc parallel loop //copyin(a[0:n],b[0:n]) copyout(c[0:n])
    for(i=0; i<n; i++) {
        c[i] = a[i] + b[i];
    }
    // Sum up vector c and print result divided by n, this should equal 1 within error
    #pragma acc parallel loop reduction(+:sum)
    for(i=0; i<n; i++) {
        sum += c[i];
    }
    }
    sum = sum/(double)n;
    printf("final result: %f\n", sum);

    // Release memory
    free(a);
    free(b);
    free(c);
 
    return 0;
}
