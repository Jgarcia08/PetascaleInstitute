/* Pi - CUDA version 2 - uses dimension variables for CUDA kernels
 * Author: Aaron Weeden, Shodor, May 2015
 */
#include <stdio.h> /* fprintf() */
#include <float.h> /* DBL_EPSILON */
#include <math.h> /* sqrt() */
#include "pi-calc-cuda.h" /* calculate1Area() */

__global__ void calculateAreas(const int numRects, const double width,
    double *dev_areas) {
  const int blockId = (blockIdx.x) +
    (blockIdx.y * gridDim.x) +
    (blockIdx.z * gridDim.x * gridDim.y);
  const int threadId = blockId * (blockDim.x * blockDim.y * blockDim.z) +
    (threadIdx.x) +
    (threadIdx.y * blockDim.x) +
    (threadIdx.z * (blockDim.x * blockDim.y));
  double x = 0.0;
  double heightSq = 0.0;
  double height = 0.0;

  if (threadId < numRects) {
    calculateHeight()
    x = (threadId * width);

    heightSq = (1.0 - (x * x));

    /* Prevent nan value for sqrt() */
    if (heightSq < DBL_EPSILON) {
      heightSq = 0.0;
    }

    height = sqrt(heightSq);

    dev_areas[threadId] = (width * height);
  }
}

void calculateArea(const int numRects, double *area) {
  const double width = (1.0 / numRects);
  const int gridDimX = 1;
  const int gridDimY = 1;
  const int gridDimZ = 1;
  const int blockDimX = 1;
  const int blockDimY = 1;
  const int blockDimZ = numRects;
  const dim3 dimGrid(gridDimX, gridDimY, gridDimZ);
  const dim3 dimBlock(blockDimX, blockDimY, blockDimZ);
  double *areas = (double*)malloc(numRects * sizeof(double));
  double *dev_areas;
  cudaError_t err;
  int i = 0;

  if (areas == NULL) {
    fprintf(stderr, "malloc failed!\n");
  }

  err = cudaMalloc((void**)&dev_areas, (numRects * sizeof(double)));

  if (err != cudaSuccess) {
    fprintf(stderr, "cudaMalloc failed: %s\n", cudaGetErrorString(err));
  }

  calculateAreas<<<dimGrid, dimBlock>>>(numRects, width, dev_areas);

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
