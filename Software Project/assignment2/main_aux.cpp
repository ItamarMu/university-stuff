/*
 * main_aux.cpp
 *
 *  Created on: Apr 21, 2016
 *      Author: project
 */
#include "main_aux.h"

#define BEST_FEATURES_NO 5

//compares two "hits" structs
int compareHits(const void *a, const void *b){
	const hits* first = (const hits*)a;
	const hits* second = (const hits*)b;
	if(first->hitsNo > second->hitsNo){
		return -1;
	}
	if(first->hitsNo == second->hitsNo){
		if(first->imgIdx < second->imgIdx){
			return -1;
		}
	}
	return 1;
}


int compare(const void *a, const void *b){
	const fDst* first = (const fDst*)a;
	const fDst* second = (const fDst*)b;
	if(first->distance < second->distance){
		return -1;
	}
	if(first->distance == second->distance){
		if(first->imgIdx < second->imgIdx){
			return -1;
		}
	}
	return 1;
}

void free2DDouble(double** arr, int length){
	for(int i=0; i<length; i++){
		free(arr[i]);
	}
	free(arr);
}

void free2DInt(int** arr, int length){
	for(int i=0; i<length; i++){
		free(arr[i]);
	}
	free(arr);
}

void free3DInt(int*** arr, int length3D,int* length2D){
	for(int i=0; i<length3D; i++){
		for(int j=0; j < length2D[i]; j++){
			free(arr[i][j]);
		}
		free(arr[i]);
	}
	free(arr);
}

void free3DDouble(double*** arr, int length3D,int* length2D){
	for(int i=0; i<length3D; i++){
		for(int j=0; j < length2D[i]; j++){
			free(arr[i][j]);
		}
		free(arr[i]);
	}
	free(arr);
}

char** pathGenerator(char* lib, char* prefix, char* suffix, int numOfImgs){
	char** paths = (char**)malloc(numOfImgs * sizeof(char*));
	for(int i=0; i<numOfImgs; i++){
		paths[i] = (char*)malloc(MAXLENGTH * sizeof(char));
	}
	for(int i=0; i<numOfImgs; i++){
		sprintf(paths[i],"%s%s%d%s",lib,prefix,i,suffix);
	}
	return paths;
}

void nearest5Global(int** queryHist,int*** histDB, int nImages, int nBins){
	fDst* RGBandIdx;
	double* RGBDistances = (double*)malloc(nImages * sizeof(double));
	alocCheck(RGBDistances);
	for (int i=0; i<nImages; i++){
		RGBDistances[i]=spRGBHistL2Distance(queryHist, histDB[i], nBins);
	}
	RGBandIdx = (struct fDst*)malloc(nImages * sizeof(struct fDst));
	alocCheck(RGBandIdx);
	for (int i=0; i<nImages; i++){
		RGBandIdx[i]={RGBDistances[i],i};
	}
	qsort(RGBandIdx, nImages, sizeof(fDst), compare);
	printf(NEAREST_GLOBAL);
	printf("%d, %d, %d, %d, %d\n",RGBandIdx[0].imgIdx,RGBandIdx[1].imgIdx,
			RGBandIdx[2].imgIdx,RGBandIdx[3].imgIdx,RGBandIdx[4].imgIdx);
	free(RGBDistances);
	free(RGBandIdx);
}

void nearest5Local(double** querySIFT,double*** siftDB, int nImages, int queryFeaturesNo, int* DBfeaturesNo){
	hits* nearest = (hits*)malloc(nImages * sizeof(hits));
	alocCheck(nearest);
	for(int i=0; i<nImages; i++){
		nearest[i] = {0,i};
	}
	for(int i=0; i<queryFeaturesNo; i++){
		int* nearestToFeature;
		nearestToFeature = spBestSIFTL2SquaredDistance(BEST_FEATURES_NO, querySIFT[i],siftDB, nImages, DBfeaturesNo);
		for(int j=0; j<BEST_FEATURES_NO; j++){
			nearest[nearestToFeature[j]].hitsNo++;
		}
		free(nearestToFeature);
	}
	qsort(nearest, nImages, sizeof(hits), compareHits);
	printf(NEAREST_LOCAL);
	printf("%d, %d, %d, %d, %d\n",nearest[0].imgIdx, nearest[1].imgIdx,
			nearest[2].imgIdx, nearest[3].imgIdx, nearest[4].imgIdx);
	free(nearest);
}


