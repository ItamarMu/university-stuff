#include "SPKDTree.h"

#define LEAF -1

struct sp_kdtree_t{
	int dim;
	double val;
	struct sp_kdtree_t* left;
	struct sp_kdtree_t* right;
	SPPoint data;
};

KDTreeNode kdTreeCreate(SPKDArray arr, KDT_SPLIT_METHOD splitMethod, int splitCoor){
	KDTreeNode node;
	SPKDArray leftArr;
	SPKDArray rightArr;
	if(!arr) return NULL;
	node = (KDTreeNode)malloc(sizeof(*node));
	if(spKDArrayGetLength(arr) == 1){
		node->dim = -1;
		node->val = NAN;
		node->left = NULL;
		node->right = NULL;
		node->data = spKDArrayGetPointAt(arr, 0);
		spKDArrayDestroy(arr);
		return node;
	}
	if(splitMethod == INCREMENTAL){
		spKDArraySplit(arr, splitCoor, &leftArr, &rightArr);
		node->dim = splitCoor;
		node->val = spKDArrayGetMedian (arr, splitCoor);
		node->left = kdTreeCreate(leftArr, splitMethod, (splitCoor + 1)%(spKDArrayGetDim(arr)));
		node->right = kdTreeCreate(rightArr, splitMethod, (splitCoor + 1)%(spKDArrayGetDim(arr)));
		node->data = NULL;
	} else { // spliting method is either RANDOM or MAX_SPREAD
		splitCoor = spKDArrayGetSplitCoor(splitMethod, arr);
		spKDArraySplit(arr, splitCoor, &leftArr, &rightArr);
		node->dim = splitCoor;
		node->val = spKDArrayGetMedian (arr, splitCoor);
		node->left = kdTreeCreate(leftArr, splitMethod, 0);
		node->right = kdTreeCreate(rightArr, splitMethod, 0);
		node->data = NULL;
	}
	spKDArrayDestroy(arr);
	return node;
}

KDTreeNode spKDTreeCreate(SPKDArray arr, KDT_SPLIT_METHOD splitMethod){
	SPKDArray copy;
	if(!arr) return NULL;
	copy = spKDArrayCopy(arr); // copy to be deallocated by kdTreeCreate
	return kdTreeCreate(copy, splitMethod, 0);
}

void kNearestNeighbors (KDTreeNode curr, SPBPQueue bpq, SPPoint P) {
	SPListElement le;
	if (curr == NULL) {
		return;
	}
	if (curr->left == NULL && curr->right == NULL) {
		le = spListElementCreate(spPointGetIndex(curr->data),spPointL2SquaredDistance(curr->data,P));
		spBPQueueEnqueue(bpq,le);
		spListElementDestroy(le);
		return;
	}
	if (spPointGetAxisCoor(P, curr->dim) <= curr->val) {
		kNearestNeighbors (curr->left, bpq, P);
	}
	else {
		kNearestNeighbors (curr->right, bpq, P);
	}
	double dis = (curr->val - spPointGetAxisCoor(P, curr->dim))*(curr->val - spPointGetAxisCoor(P, curr->dim));
	if (!spBPQueueIsFull(bpq) || dis < spBPQueueMaxValue(bpq)) {//MIGHT NEED TO CHANGE MAX TO MIN
		if (spPointGetAxisCoor(P, curr->dim) <= curr->val)
			kNearestNeighbors (curr->right, bpq, P);
		else
			kNearestNeighbors (curr->left, bpq, P);
	}
}

void spKDTreeDestroy(KDTreeNode tree){
	if(tree){
		if(tree->left) spKDTreeDestroy(tree->left);
		if(tree->right) spKDTreeDestroy(tree->right);
		if(tree->dim == LEAF) spPointDestroy(tree->data);
		free(tree);
	}
}

