/* HPCU Bi-Weekly Challenge: __CUDACC__ Fire
 * Author: Aaron Weeden, Shodor, April 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FOREST_ROW_COUNT 21
#define FOREST_COLUMN_COUNT 21
#define TREE_COUNT (FOREST_ROW_COUNT * FOREST_COLUMN_COUNT)
#define TIME_STEP_COUNT 100
#define PROBABILITY_OF_BURN 100 /* Should be between 0 and 100 */
#define MAX_BURN_TIME 5

#ifdef __CUDACC__
__host__ __device__
#endif
int isOnFire(const int *forest, const int treeIdx) {
  return forest[treeIdx] > 0;
}

#ifdef __CUDACC__
__host__ __device__
#endif
int isBurntOut(const int *forest, const int treeIdx) {
  return forest[treeIdx] >= MAX_BURN_TIME;
}

void initializeForest(int *currentForest, int *nextForest) {
  int treeIdx;

  /* Start by assuming no trees are burning */
#pragma omp parallel for
  for (treeIdx = 0; treeIdx < TREE_COUNT; treeIdx++) {
    currentForest[treeIdx] = 0;
  }

  /* Light a random tree on fire */
  currentForest[random() % TREE_COUNT] = 1;

  /* Make a copy of the forest */
#pragma omp parallel for
  for (treeIdx = 0; treeIdx < TREE_COUNT; treeIdx++) {
    nextForest[treeIdx] = currentForest[treeIdx];
  }
}

void displayForest(const int *forest, const int timeIdx) {
  int rowIdx;
  int colIdx;

  printf("Time step %d\n", timeIdx);
  for (rowIdx = 0; rowIdx < FOREST_ROW_COUNT; rowIdx++) {
    for (colIdx = 0; colIdx < FOREST_COLUMN_COUNT; colIdx++) {
      printf("%d ", forest[rowIdx * FOREST_COLUMN_COUNT + colIdx]);
    }
    printf("\n");
  }
  printf("\n");
}

/* Continue to burn trees if they are on fire and haven't burnt out */
#ifdef __CUDACC__
__global__
#endif
void advanceBurningTrees(const int *currentForest, int *nextForest) {
  int treeIdx;

#ifdef __CUDACC__
  treeIdx = blockIdx.x;
  if (treeIdx < TREE_COUNT) {
#else
  #pragma omp parallel for
  for (treeIdx = 0; treeIdx < TREE_COUNT; treeIdx++) {
#endif
    if ((isOnFire(currentForest, treeIdx)) &&
        (!isBurntOut(currentForest, treeIdx))) {
      nextForest[treeIdx] = 1 + currentForest[treeIdx];
    }
  }
}

/* Given a condition to check, the index of a tree, and the index of a neighbor
 * tree, check if the neighbor tree is on fire (but only if the condition is
 * true), and if it is, randomly spread the fire from that neighbor tree */
void trySpread(const int treeIdx, const int condition,
    const int neighborTreeIdx, const int *currentForest, int *nextForest) {
  if ((condition) &&
      (isOnFire(currentForest, neighborTreeIdx)) &&
      ((random() % 100) < PROBABILITY_OF_BURN)) {
    nextForest[treeIdx] = 1;
  }
}

void burnNewTrees(const int *currentForest, int *nextForest) {
  int treeIdx;

  /* Find trees that are not on fire yet and try to catch them on fire from
   * burning neighbor trees */
#pragma omp parallel for
  for (treeIdx = 0; treeIdx < TREE_COUNT; treeIdx++) {
    if (!isOnFire(currentForest, treeIdx)) {

      /* Top neighbor */
      trySpread(treeIdx,
        treeIdx >= FOREST_COLUMN_COUNT,
        treeIdx - FOREST_COLUMN_COUNT,
        currentForest, &(*nextForest));

      /* Left neighbor */
      trySpread(treeIdx,
        (treeIdx % FOREST_COLUMN_COUNT) > 0,
        treeIdx - 1,
        currentForest, &(*nextForest));

      /* Bottom neighbor */
      trySpread(treeIdx,
        treeIdx < (TREE_COUNT - FOREST_COLUMN_COUNT),
        treeIdx + FOREST_COLUMN_COUNT,
        currentForest, &(*nextForest));

      /* Right neighbor */
      trySpread(treeIdx,
        (treeIdx % FOREST_COLUMN_COUNT) < (FOREST_COLUMN_COUNT - 1),
        treeIdx + 1,
        currentForest, &(*nextForest));
    }
  }
}

int main() {
  /* Keep track of how many time steps each tree in the forest has been
   * burning. We need a "current" and "next" so that we don't overwrite the state
   * of one tree while we are checking the state of another tree. */
  int *currentForest;
  int *nextForest;
#ifdef __CUDACC__
  int *dev_currentForest;
  int *dev_nextForest;
#endif

  int timeIdx;
  int treeIdx;

  /* Seed the random number generator with the current time */
  srandom(time(NULL));

  /* Allocate memory */
  currentForest = (int*)malloc(TREE_COUNT * sizeof(int));
  nextForest = (int*)malloc(TREE_COUNT * sizeof(int));
#ifdef __CUDACC__
  cudaMalloc((void**)&dev_currentForest, TREE_COUNT * sizeof(int));
  cudaMalloc((void**)&dev_nextForest, TREE_COUNT * sizeof(int));
#endif

  initializeForest(&(*currentForest), &(*nextForest));

  for (timeIdx = 1; timeIdx <= TIME_STEP_COUNT; timeIdx++) {
    /* These three steps can happen in any order; they are independent of each
     * other */
    displayForest(currentForest, timeIdx);

#ifdef __CUDACC__
    cudaMemcpy(dev_currentForest, currentForest, TREE_COUNT * sizeof(int),
      cudaMemcpyHostToDevice);
    cudaMemcpy(dev_nextForest, nextForest, TREE_COUNT * sizeof(int),
      cudaMemcpyHostToDevice);

    advanceBurningTrees<<<TREE_COUNT, 1>>>(dev_currentForest, dev_nextForest);

    cudaMemcpy(nextForest, dev_nextForest, TREE_COUNT * sizeof(int),
      cudaMemcpyDeviceToHost);
#else
    advanceBurningTrees(currentForest, &(*nextForest));
#endif

    burnNewTrees(currentForest, &(*nextForest));

    for (treeIdx = 0; treeIdx < TREE_COUNT; treeIdx++) {
      currentForest[treeIdx] = nextForest[treeIdx];
    }
  }

  /* Free memory */
#ifdef __CUDACC__
  cudaFree(dev_currentForest);
  cudaFree(dev_nextForest);
#endif
  free(nextForest);
  free(currentForest);

  return 0;
}
