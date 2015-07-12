#include <stdio.h> /* fprintf() */
#include <float.h> /* DBL_EPSILON and LDBL_DIG */
#include <math.h> /* sqrt() */

__global__ void calculateAreas(const int myNumRects, const double width,
    const int myDispl, double *dev_areas) {
  const int blockId = (blockIdx.x) +
    (blockIdx.y * gridDim.x) +
    (blockIdx.z * gridDim.x * gridDim.y);
  const int threadId = blockId * (blockDim.x * blockDim.y * blockDim.z) +
    (threadIdx.x) +
    (threadIdx.y * blockDim.x) +
    (threadIdx.z * (blockDim.x * blockDim.y));
  const double x = ((myDispl + threadId) * width);
  const double heightSq = (1.0 - (x * x));
  /* Prevent nan value for sqrt() */
  const double height = (heightSq < DBL_EPSILON) ? (0.0) : (sqrt(heightSq));

  if (threadId < (myDispl + myNumRects)) {
    dev_areas[threadId] = (width * height);
  }
}

void calculateArea(const int myNumRects, const double width, const int myDispl,
    double *area) {
  const int gridDimX = 1;
  const int gridDimY = 1;
  const int gridDimZ = 1;
  const int blockDimX = 1;
  const int blockDimY = 1;
  const int blockDimZ = myNumRects;
  const dim3 dimGrid(gridDimX, gridDimY, gridDimZ);
  const dim3 dimBlock(blockDimX, blockDimY, blockDimZ);
  double *areas = (double*)malloc(myNumRects * sizeof(double));
  double *dev_areas;
  cudaError_t err;
  int i = 0;

  if (areas == NULL) {
    fprintf(stderr, "malloc failed!\n");
  }

  err = cudaMalloc((void**)&dev_areas, (myNumRects * sizeof(double)));

  if (err != cudaSuccess) {
    fprintf(stderr, "cudaMalloc failed: %s\n", cudaGetErrorString(err));
  }

  calculateAreas<<<dimGrid, dimBlock>>>(myNumRects, width, myDispl, dev_areas);

  err = cudaMemcpy(areas, dev_areas, (myNumRects * sizeof(double)),
    cudaMemcpyDeviceToHost);

  if (err != cudaSuccess) {
    fprintf(stderr, "cudaMalloc failed: %s\n", cudaGetErrorString(err));
  }

  (*area) = 0.0;
  for (i = 0; i < myNumRects; i++) {
    (*area) += areas[i];
  }

  cudaFree(dev_areas);

  free(areas);
}
