/*
 * SPBPriorityQueue.c
 *
 *  Created on: May 29, 2016
 *      Author: project
 */
#include "SPBPriorityQueue.h"
#include "SPList.h"
#include "assert.h"
#include <stdlib.h>

struct sp_bp_queue_t{
	int maxSize;
	SPList list;
};

SPBPQueue spBPQueueCreate(int maxSize) {
	if(maxSize <=0){
		return NULL;
	}
	SPBPQueue res;
	res = (SPBPQueue) malloc(sizeof(*res));
	if (res == NULL) {
		spBPQueueDestroy(res);
		return NULL;
	}
	res->maxSize = maxSize;
	res->list = spListCreate();
	if (res->list == NULL) {
		spBPQueueDestroy(res);
		return NULL;
	}
	return res;
}

SPBPQueue spBPQueueCopy(SPBPQueue source){
	if(source == NULL){
		return NULL;
	}
	SPBPQueue new = spBPQueueCreate(source->maxSize);
	if(new == NULL){
		return NULL;
	}
	SPList listCopy =  spListCopy(source->list);
	if(listCopy == NULL){
		return NULL;
	}
	spListDestroy(new->list);
	new->list = listCopy;
	return new;
}

void spBPQueueDestroy(SPBPQueue source) {
	if (source == NULL) {
		return;
	}
	spListDestroy(source->list);
	free(source);
	return;
}

void spBPQueueClear(SPBPQueue source){
	if(source == NULL){
		return;
	}
	spListClear(source->list);
}

int spBPQueueSize(SPBPQueue source) {
	if(source == NULL){
		return -1;
	}
	return spListGetSize(source->list);
}

int spBPQueueGetMaxSize(SPBPQueue source){
	if(source == NULL){
		return -1;
	}
	return source->maxSize;
}

SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element) {
	if (source == NULL || element == NULL) {
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	if (spBPQueueSize(source) >= source->maxSize) {
		if(spListElementGetValue(element) > spListElementGetValue(spListGetLast(source->list))){
			return SP_BPQUEUE_FULL;
		}
		if(spListElementGetValue(element) == spListElementGetValue(spListGetLast(source->list))){
			if(spListElementGetIndex(element) >= spListElementGetIndex(spListGetLast(source->list))){
				return SP_BPQUEUE_FULL;
			}
		}
		spListGetLast(source->list);
		spListRemoveCurrent(source->list);
	}
	SPListElement tmp = spListGetFirst(source->list);
	while (tmp != NULL) {
		if (spListElementGetValue(tmp) > spListElementGetValue(element)) {
			SP_LIST_MSG msg = spListInsertBeforeCurrent(source->list, element);
			if (msg == SP_LIST_OUT_OF_MEMORY) {
				return SP_BPQUEUE_OUT_OF_MEMORY;
			}
			return SP_BPQUEUE_SUCCESS;
		}
		if(spListElementGetValue(tmp) == spListElementGetValue(element)){
			if(spListElementGetIndex(tmp) > spListElementGetIndex(element)){
				SP_LIST_MSG msg = spListInsertBeforeCurrent(source->list, element);
				if (msg == SP_LIST_OUT_OF_MEMORY) {
					return SP_BPQUEUE_OUT_OF_MEMORY;
				}
				return SP_BPQUEUE_SUCCESS;
			}
		}
		tmp = spListGetNext(source->list);
	}
	spListInsertLast(source->list,element);
	return SP_BPQUEUE_SUCCESS;
}

SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source){
	if(source == NULL){
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	SPListElement first = spListGetFirst(source->list);
	if(first == NULL){
		return SP_BPQUEUE_EMPTY;
	}
	spListRemoveCurrent(source->list);
	return SP_BPQUEUE_SUCCESS;
}

SPListElement spBPQueuePeek(SPBPQueue source){
		if(source == NULL){
			return NULL;
		}
		SPListElement first = spListGetFirst(source->list);
		if(first == NULL){
				return NULL;
		}
		SPListElement copy = spListElementCopy(first);
		if(copy == NULL){
			return NULL;
		}
		return copy;
}

SPListElement spBPQueuePeekLast(SPBPQueue source){
	if(source == NULL){
		return NULL;
	}
	SPListElement last = spListGetLast(source->list);
	if(last == NULL){
			return NULL;
	}
	SPListElement copy = spListElementCopy(last);
	if(copy == NULL){
		return NULL;
	}
	return copy;
}

double spBPQueueMinValue(SPBPQueue source){
	if(source == NULL){
		return -1;
	}
	SPListElement first = spListGetFirst(source->list);
	if(first == NULL){
		return -1;
	}
	return spListElementGetValue(first);
}

double spBPQueueMaxValue(SPBPQueue source){
	if(source == NULL){
		return -1;
	}
	SPListElement last = spListGetLast(source->list);
	if(last == NULL){
		return -1;
	}
	return spListElementGetValue(last);
}

bool spBPQueueIsFull(SPBPQueue source){
	assert(source != NULL);
	if(spListGetSize(source->list) == source->maxSize){
		return true;
	}
	return false;
}

bool spBPQueueIsEmpty(SPBPQueue source){
	assert(source != NULL);
	if(spListGetSize(source->list) == 0){
		return true;
	}
	return false;
}

