#include "SPKDArray.h"

typedef struct sp_sorted_idx_cell_t{
	SPPoint point;
	int coor;
	int index;
}SortedIdxCell;

struct sp_KDArray_t{
	SPPoint* arr;
	SortedIdxCell** sortedIdx;
	int length;
	int dim;
};

/**
 * Compares between two points.
 *
 * This function is used by qsort to sort arrays by coordinate
 */
int cmpPoints(const void* a, const void *b){
	SortedIdxCell* left = (SortedIdxCell*)a;
	SortedIdxCell* right = (SortedIdxCell*)b;
	double res = spPointGetAxisCoor(left->point,left->coor) - spPointGetAxisCoor(right->point,right->coor);
	if(res > 0){
		return 1;
	}
	if(res < 0){
		return -1;
	}
	return 0;
}

SPKDArray spKDArrayCreate(SPPoint* arr, int size, int dim){
	SPKDArray res = (SPKDArray)malloc(sizeof(*res));
	if(!res){
		return NULL;
	}
	res->arr = (SPPoint*)malloc(sizeof(SPPoint) * size);
	if(!res->arr){
		return NULL;
	}
	res->sortedIdx = (SortedIdxCell**)malloc(sizeof(SortedIdxCell*) * dim);
	if(!res->sortedIdx){
			return NULL;
	}
	for(int i=0; i<dim; i++){
		res->sortedIdx[i] = (SortedIdxCell*)malloc(sizeof(SortedIdxCell) * size);
		if(!res->sortedIdx[i]){
				return NULL;
		}
	}
	for(int i=0;i<size;i++){
		res->arr[i] = spPointCopy(arr[i]);
	}
	for(int i=0; i<dim; i++){
		for(int j=0; j<size; j++){
			SortedIdxCell cell = {.point = res->arr[j], .coor = i, .index = j};
			res->sortedIdx[i][j] = cell;
		}
		qsort(res->sortedIdx[i], size, sizeof(SortedIdxCell), cmpPoints);
	}
	res->length = size;
	res->dim = dim;
	return res;
}

void spKDArraySplit(SPKDArray arr, int coor, SPKDArray* leftPtr, SPKDArray* rightPtr){
	int length = arr->length;
	int leftLength = length/2 + length%2;
	int rightLength = length - leftLength;
	int dim = arr->dim;
	SPKDArray leftKDArr;
	SPKDArray rightKDArr;
	SPPoint* leftArr;
	SPPoint* rightArr;
	SortedIdxCell**leftMat;
	SortedIdxCell**rightMat;
	int* isLeft = (int*)malloc(sizeof(int) * length);
	int* newIndexMap = (int*)malloc(sizeof(int) * length);
	//init left and right arrays
	leftArr = (SPPoint*)malloc(sizeof(*leftArr) * leftLength);
	rightArr = (SPPoint*)malloc(sizeof(*rightArr) * rightLength);
	//init left and right mats
	leftMat = (SortedIdxCell**)malloc(sizeof(*leftMat) * dim);
	for(int i=0; i<dim; i++){
			leftMat[i] = (SortedIdxCell*)malloc(sizeof(SortedIdxCell) * leftLength);
		if(!leftMat[i]){
				//logger
		}
	}
	rightMat = (SortedIdxCell**)malloc(sizeof(*rightMat) * dim);
	for(int i=0; i<dim; i++){
			rightMat[i] = (SortedIdxCell*)malloc(sizeof(SortedIdxCell) * rightLength);
		if(!rightMat[i]){
				//logger
		}
	}
	//init left and right KD Arrays
	leftKDArr = (SPKDArray)malloc(sizeof(*leftKDArr));
	rightKDArr = (SPKDArray)malloc(sizeof(*leftKDArr));
	leftKDArr->arr = leftArr;
	leftKDArr->dim = dim;
	leftKDArr->length = leftLength;
	leftKDArr->sortedIdx = leftMat;
	rightKDArr->arr = rightArr;
	rightKDArr->dim = dim;
	rightKDArr->length = rightLength;
	rightKDArr->sortedIdx = rightMat;
	//sort left and right using the original sorted array

	//init two hashmaps, points array.
	for(int i=0; i<length; i++){
		if(i < leftLength){
			isLeft[arr->sortedIdx[coor][i].index] = 1;
			leftKDArr->arr[i] = spPointCopy(arr->sortedIdx[coor][i].point);
			newIndexMap[arr->sortedIdx[coor][i].index] =  i;
		} else {
			isLeft[arr->sortedIdx[coor][i].index] = 0;
			rightKDArr->arr[i - leftLength] = spPointCopy(arr->sortedIdx[coor][i].point);
			newIndexMap[arr->sortedIdx[coor][i].index] =  i - leftLength;
		}
	}

	//copy values to left and right mats
	for(int i=0; i<dim; i++){
		int leftIdx = 0;
		int rightIdx = 0;
		for(int j=0; j<length; j++){
			if(isLeft[arr->sortedIdx[i][j].index]){
				SortedIdxCell cell = {.point = leftKDArr->arr[newIndexMap[arr->sortedIdx[i][j].index]],
									.coor = i,
									.index = newIndexMap[arr->sortedIdx[i][j].index]
									};
				leftKDArr->sortedIdx[i][leftIdx] = cell;
				leftIdx++;
			} else {
				SortedIdxCell cell = {.point = rightKDArr->arr[newIndexMap[arr->sortedIdx[i][j].index]],
									.coor = i,
									.index = newIndexMap[arr->sortedIdx[i][j].index]
									};
				rightKDArr->sortedIdx[i][rightIdx] = cell;
				rightIdx++;
			}
		}
	}
	free(isLeft);
	free(newIndexMap);
	*leftPtr = leftKDArr;
	*rightPtr = rightKDArr;
}

int spKDArrayGetLength(SPKDArray arr){
	if(!arr) return -1;
	return arr->length;
}

int spKDArrayGetDim(SPKDArray arr){
	if(!arr) return -1;
	return arr->dim;
}

SPPoint spKDArrayGetPointAt(SPKDArray arr, int i){
	if(!arr || i<0 || i >= arr->length) return NULL;
	return spPointCopy(arr->arr[i]);
}

int spKDArrayGetSplitCoor (KDT_SPLIT_METHOD method , SPKDArray KDarr) {
	int dim = 0;
	if (method == RANDOM) {
		dim = rand() % KDarr->dim;
		return dim;
	}
	double maxSub = 0, max = 0, min = 0, sub = 0;
	if (method == MAX_SPREAD) {
		for (int i = 0; i < KDarr->dim; i++) {
			min = spPointGetAxisCoor(KDarr->arr[KDarr->sortedIdx[i][0].index],i);
			max = spPointGetAxisCoor(KDarr->arr[KDarr->sortedIdx[i][(KDarr->length)-1].index],i);
			sub = max - min;
			if (maxSub < sub) {
				maxSub = sub;
				dim = i;
			}
		}
		return dim;

	}
	return -1;
}

/**
 * Frees a 2D SortedIdxCell array
 *
 * This function is used to free a 2D SortedIdxCell array
 */
void free2DSICell(SortedIdxCell** arr, int rows){
	for(int i=0; i<rows; i++){
		free(arr[i]);
	}
	free(arr);
}

void freePointArray(SPPoint* arr, int length){
	if(!arr) return;
	for(int i=0; i<length; i++){
		spPointDestroy(arr[i]);
	}
	free(arr);
}

void spKDArrayDestroy(SPKDArray kdarr){
	if(kdarr){
		freePointArray(kdarr->arr, kdarr->length);
		free2DSICell(kdarr->sortedIdx, kdarr->dim);
		free(kdarr);
	}
}

double spKDArrayGetMedian (SPKDArray arr, int coor){
	if(!arr || coor < 0 || coor >= arr->dim) return NAN;
	int medIdx = arr->length/2;
	return spPointGetAxisCoor(arr->sortedIdx[coor][medIdx].point, coor);
}

/**
 * Creates a copy of a given point array.
 *
 * The new copy will be a point array with exactly the same stats as the given point array, but a copy
 * pointing to another place in the memory
 *
 * @param old target point array to be copied.
 * @return
 * NULL if a NULL was sent or a memory allocation failed.
 * A point array containing the same stats as the target point array otherwise.
 */
SPPoint* copyPointArr(SPPoint* old, int length){
	if(old){
		SPPoint* new = (SPPoint*)malloc(sizeof(*new) * length);
		if (!new) {
			return NULL;
		}
		for(int i=0; i<length; i++){
			new[i] = spPointCopy(old[i]);
		}
		return new;
	}
	return NULL;
}

/**
 * Creates a copy of a given 2D SortedIdxCell array.
 *
 * The new copy will be a 2D SortedIdxCell array with exactly the same stats as the given 2D SortedIdxCell array, but a copy
 * pointing to another place in the memory
 *
 * @param old target 2D SortedIdxCell array to be copied.
 * @return
 * NULL if a NULL was sent or a memory allocation failed.
 * A 2D SortedIdxCell array containing the same stats as the target 2D SortedIdxCell array otherwise.
 */
SortedIdxCell** copySICell2DArray(SortedIdxCell** old, int rows, int cols){
	if(old){
		SortedIdxCell** new = (SortedIdxCell**)malloc(sizeof(*new) * rows);
		if (!new) {
			return NULL;
		}
		for(int i=0; i<rows; i++){
			new[i] = (SortedIdxCell*)malloc(sizeof(**new) * cols);
			if (!new[i]){
				return NULL;
			}
			for(int j=0; j<cols; j++){
				new[i][j] = old[i][j];
			}
		}
		return new;
	}
	return NULL;
}

SPKDArray spKDArrayCopy(SPKDArray old){
	if(old){
		SPKDArray new = (SPKDArray)malloc(sizeof(*new));
		new->dim = old->dim;
		new->length = old->length;
		new->arr = copyPointArr(old->arr, old->length);
		if(!new->arr) return NULL;
		new->sortedIdx = copySICell2DArray(old->sortedIdx, old->dim, old->length);
		if(!new->sortedIdx){
			freePointArray(new->arr, new->length);
			return NULL;
		}
		return new;
	}
	return NULL;
}
