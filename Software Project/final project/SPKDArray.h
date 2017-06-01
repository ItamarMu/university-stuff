#ifndef SPKDTREE_C_
#define SPKDTREE_C_
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "SPPoint.h"
#include "SPConfig.h"

/** type used to define KD array **/
typedef struct sp_KDArray_t* SPKDArray;

/**
 * Allocates a new KD array.
 *
 * This function creates a new KD array using an array of points.
 * @param arr an array of points to be used to make the KD array
 * @param size length of the new KD array
 * @param dim dimension of the new KD array
 * @return
 * 	NULL - If allocations failed.
 * 	A new KD array in case of success.
 */
SPKDArray spKDArrayCreate(SPPoint* arr, int size, int dim);

/**
 * Splits a KD array to two KD arrays.
 *
 * This function splits a KD array to two parts and makes the pointers given as parameters point to them.
 * @param arr a KD array
 * @param coor the KD array will be split according to this coordinate
 * @param leftPtr this pointer will point to the left part of the split KD array
 * @param rightPtr this pointer will point to the right part of the split KD array
 *
 */
void spKDArraySplit(SPKDArray arr, int coor, SPKDArray* leftPtr, SPKDArray* rightPtr);

/**
 * Gets the point at a given index
 *
 * This function returns a copy of a point at a given index in the SPKDArray.
 * @param arr target KD array
 * @param i the index to be used
 * @return
 * 	NULL - If arr points to NULL or i<0 or i>=the array's length
 * 	The point at index i in case of success.
 */
SPPoint spKDArrayGetPointAt(SPKDArray arr, int i);

/**
 * Gets the length of a given KD array
 *
 * This function returns the length of a given KD array.
 * @param arr target KD array
 * @return
 * 	-1 - If arr points to NULL
 * 	The length of a given KD array in case of success.
 */
int spKDArrayGetLength(SPKDArray arr);

/**
 * Gets the dimension of a given KD array
 *
 * This function returns the dimension of a given KD array.
 * @param arr target KD array
 * @return
 * 	-1 - If arr points to NULL
 * 	The dimension of a given KD array in case of success.
 */
int spKDArrayGetDim(SPKDArray arr);

/**
 * Gets the dimension to split at given a splitting method RANDOM or MAX_SPREAD, INCREMENTAL is done at main function
 *
 * This function returns dimension to split at given a splitting method.
 * @param method the spltting method
 * @param arr target KD array
 * @return
 *  random dimension if method is RANDOM
 *  max spread dimension if method is MAX_SPREAD
 *  -1 otherwise
 */
int spKDArrayGetSplitCoor (KDT_SPLIT_METHOD method , SPKDArray KDarr);

/**
 * Gets the median value according to a given dimension
 *
 * This function returns the median value according to a given dimension.
 * @param arr target KD array
 * @param coor a given dimension
 * @return
 *  NAN - If arr points to NULL or coor<0 or coor>=the array's dimension
 *  The median value according to a given dimension otherwise
 */
double spKDArrayGetMedian (SPKDArray arr, int coor);

/**
 * Deallocates an existing KD array.
 *
 * This function clears all fields and memory used by a given KD array using free2DSICell, freePointArray and 'free' functions
 * @param kdarr target KD array to be deallocated. If kdarr is NULL nothing will be done
 */
void spKDArrayDestroy(SPKDArray kdarr);

/**
 * Deallocates an existing point array.
 *
 * This function frees the memory used by a point array using 'free' and spPointDestroy functions
 * @param arr target point array to be deallocated.
 * @param length arr's length
 */
void freePointArray(SPPoint* arr, int length);

/**
 * Creates a copy of a given KD array.
 *
 * The new copy will be a KD array with exactly the same stats as the given KD array, but a copy
 * pointing to another place in the memory
 *
 * @param old target KD array to be copied.
 * @return
 * NULL if a NULL was sent or a memory allocation failed.
 * A KD array containing the same stats as the target KD array otherwise.
 */
SPKDArray spKDArrayCopy(SPKDArray old);

#endif /* SPKDTREE_C_ */
