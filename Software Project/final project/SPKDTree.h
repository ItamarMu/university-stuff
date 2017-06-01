/*
 * SPKDTree.h
 *
 *  Created on: Aug 6, 2016
 *      Author: project
 */

#ifndef SPKDTREE_H_
#define SPKDTREE_H_
#include <math.h>
#include "SPKDArray.h"
#include "SPConfig.h"
#include "SPBPriorityQueue.h"
#include "SPListElement.h"

/**
 * A data-structure which is used for creating a KDTREE.
 */
typedef struct sp_kdtree_t* KDTreeNode;

/**
 * Creates a new KD Tree using a given KD Array.
 *
 * @param arr - the name of the KD Array
 * @param KDT_SPLIT_METHOD - the split method for building the KD Tree.
 * @return NULL in case an error occurs. Otherwise, a pointer to a struct which
 * 		   contains the KD Tree.
 */
KDTreeNode spKDTreeCreate(SPKDArray arr, KDT_SPLIT_METHOD splitMethod);

/**
 * Creates a queue with the SPKNN nearest neighbors within the KD Tree to a given feature.
 *
 * @param curr - the name of the KD Tree.
 * @param bpq - a pointer in which the queue will be saved
 * @param P - the given feature.
 *
 */
void kNearestNeighbors (KDTreeNode curr, SPBPQueue bpq, SPPoint P);

/**
 * Frees all memory resources associate with SPKDTree.
 * If tree == NULL nothing is done.
 */
void spKDTreeDestroy(KDTreeNode tree);

#endif /* SPKDTREE_H_ */
