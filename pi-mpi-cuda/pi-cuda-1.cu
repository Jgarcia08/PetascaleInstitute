/* Pi - CUDA version 1 - uses integers for CUDA kernels
 * Author: Aaron Weeden, Shodor, May 2015
 */
#include <stdio.h> /* fprintf() */
#include <float.h> /* DBL_EPSILON() */
#include <math.h> /* sqrt() */

__global__ void calculateAreas(const int numRects, const double width,
    double *dev_areas) {
  const int threadId = threadIdx.x;
  const double x = (threadId * width);
  const double heightSq = (1.0 - (x * x));
  const double height =
    /* Prevent nan value for sqrt() */
    (heightSq < DBL_EPSILON) ? (0.0) : (sqrt(heightSq));

  if (threadId < numRects) {
    dev_areas[threadId] = (width * height);
  }
}

void calculateArea(const int numRects, double *area) {
  double *areas = (double*)malloc(numRects * sizeof(double));
  double *dev_areas;
  int i = 0;
  cudaError_t err;

  if (areas == NULL) {
    fprintf(stderr, "malloc failed!\n");
  }

  err = cudaMalloc((void**)&dev_areas, (numRects * sizeof(double)));

  if (err != cudaSuccess) {
    fprintf(stderr, "cudaMalloc failed: %s\n", cudaGetErrorString(err));
  }

  calculateAreas<<<1, numRects>>>(numRects, (1.0 / numRects), dev_areas);

  err = cudaMemcpy(areas, dev_areas, (numRects * sizeof(double)),
    cudaMemcpyDeviceToHost);

  if (err != cudaSuccess) {
    fprintf(stderr, "cudaMalloc failed: %s\n", cudaGetErrorString(err));
  }

  (*area) = 0.0;
  for (i = 0; i < numRects; i++) {
    (*area) += areas[i];
  }

  cudaFree(dev_areas);

  free(areas);
}
