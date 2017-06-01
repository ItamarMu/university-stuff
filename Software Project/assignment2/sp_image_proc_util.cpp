/*
 * sp_image_proc_util.cpp
 *
 *  Created on: Apr 21, 2016
 *      Author: project
 */

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/features2d.hpp>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "sp_image_proc_util.h"
#include "main_aux.h"
#define PIXEL_RANGE 256
#define THIRD 0.33


using namespace cv;
using namespace std;

int** spGetRGBHist(char* str, int nBins){
		Mat src;
		/// Load image
		src = imread(str, CV_LOAD_IMAGE_COLOR);
		if (src.empty() || nBins <= 0) {
			return NULL;
		}
		/// Separate the image in 3 places ( B, G and R )
		std::vector<Mat> bgr_planes;
		split(src, bgr_planes);
		/// Set the ranges ( for B,G,R) )
		float range[] = { 0, PIXEL_RANGE };
		const float* histRange = { range };
		/// Set the other parameters:
		int nImages = 1;
		//Output
		Mat b_hist, g_hist, r_hist;
		/// Compute the histograms:
		/// The results will be store in b_hist,g_hist,r_hist.
		/// The output type of the matrices is CV_32F (float)
		calcHist(&bgr_planes[0], nImages, 0, Mat(), b_hist, 1, &nBins, &histRange);
		calcHist(&bgr_planes[1], nImages, 0, Mat(), g_hist, 1, &nBins, &histRange);
		calcHist(&bgr_planes[2], nImages, 0, Mat(), r_hist, 1, &nBins, &histRange);

		int **hist = (int **)malloc(RGB_RANGE * sizeof(int *));
		alocCheck(hist);
		for (int i = 0; i < RGB_RANGE; i++) {
			hist[i] = (int *)malloc(nBins * sizeof(int));
			alocCheck(hist[i]);
		}

		for(int i=0; i<nBins; i++){
			hist[0][i] = (int)r_hist.at<float>(i);
		}

		for(int i=0; i<nBins; i++){
			hist[1][i] = (int)g_hist.at<float>(i);
		}

		for(int i=0; i<nBins; i++){
			hist[2][i] = (int)b_hist.at<float>(i);
		}
		return hist;
}

double spRGBHistL2Distance(int** histA, int** histB, int nBins){
	double dist = 0;
	if(histA == NULL || histB == NULL || nBins <= 0){
		return -1;
	}

	for(int i=0; i<RGB_RANGE; i++){
		for(int j=0; j<nBins; j++){
			dist += pow(histA[i][j] - histB[i][j],2);
		}
	}

	return dist * THIRD;
}

double** spGetSiftDescriptors(char* str, int maxNFeautres, int *nFeatures){
	Mat src;
	src = imread(str, CV_LOAD_IMAGE_GRAYSCALE);
	if(str == NULL || nFeatures == NULL || maxNFeautres <= 0 || src.empty()){
		return NULL;
	}

	vector<KeyPoint> keyPoints;
	cv::Mat features;
	//Creating  a Sift Descriptor extractor
	cv::Ptr<cv::xfeatures2d::SiftDescriptorExtractor> detect =
			xfeatures2d::SIFT::create(maxNFeautres);
	//Extracting features
	//The features will be stored in ds1
	//The output type of ds1 is CV_32F (float)
	detect->detect(src, keyPoints, cv::Mat());
	detect->compute(src, keyPoints, features);
	*nFeatures = features.rows;

	double **sift = (double **)malloc(*nFeatures * sizeof(double *));
	alocCheck(sift);
	for (int i = 0; i < *nFeatures; i++) {
				sift[i] = (double *)malloc(128 * sizeof(double));
				alocCheck(sift[i]);
	}

	for(int i=0; i< *nFeatures; i++){
		for(int j=0; j<128; j++){
			sift[i][j] = (double)features.at<float>(i,j);
//			printf("row: %d, col: %d, value: %f\n",i,j,sift[i][j]); - for Debugging
		}
	}
	return sift;
}

double spL2SquaredDistance(double* featureA, double* featureB) {
	double dis;
	if(featureA == NULL || featureB == NULL){
		return -1;
	}
	dis = 0;
	for (int i=0; i<128; i++) {
		dis += pow(featureA[i]-featureB[i],2);
	}
	return dis;
}

// spBestSIFTL2SquaredDistance Implementation Section

// returns the index of the fDst struct with the maximum distance in an array
int max(fDst* arr, int arrLength){
	int idx = -1;
	double max = -1;
	for(int i=0; i<arrLength; i++){
		if(arr[i].distance > max){
			idx = i;
			max = arr[i].distance;
		}
	}
	return idx;
}

int* spBestSIFTL2SquaredDistance(int bestNFeatures, double* featureA,
		double*** databaseFeatures, int numberOfImages,
		int* nFeaturesPerImage)
	{
	int bnfIndex;
	int* result;
	if(featureA == NULL || databaseFeatures == NULL || numberOfImages <= 1 || nFeaturesPerImage == NULL){
		return NULL;
	}

	struct fDst* bnf = (struct fDst*)malloc(bestNFeatures * sizeof(struct fDst));
	alocCheck(bnf);
	bnfIndex = 0;
	for(int i=numberOfImages - 1; i>= 0; i--){
		for(int j=0; j<nFeaturesPerImage[i]; j++){
			double dst = spL2SquaredDistance(featureA,databaseFeatures[i][j]);
			// bnf init
			if(bnfIndex < bestNFeatures){
				bnf[bnfIndex++] = {dst, i};
			// insert smaller dst. features into bnf
			} else {
				int maxIndex = max(bnf,bestNFeatures);
				if(bnf[maxIndex].distance >= dst){
					bnf[maxIndex] = {dst, i};
				}
			}
		}
	}
	// sort bnf
	qsort(bnf, bestNFeatures, sizeof(fDst), compare);
	result = (int*)malloc(sizeof(int) * bestNFeatures);
	alocCheck(result);
	for(int i=0; i<bestNFeatures; i++){
		result[i] = bnf[i].imgIdx;
	}
	free(bnf);
	return result;
}
