/*
 * main_aux.h
 *
 *  Created on: Apr 21, 2016
 *      Author: project
 */

#ifndef MAIN_AUX_H_
#include <stdio.h>
#include <stdlib.h>
#include "sp_image_proc_util.h"
#define MAIN_AUX_H_
#define MAXLENGTH 1024
#define EXIT_MSG "Exiting...\n"
#define IMG_PATH_MSG "Enter images directory path:\n"
#define IMG_PREF_MSG "Enter images prefix:\n"
#define IMG_NUM_MSG "Enter number of images:\n"
#define IMG_INVALID_NUM_MSG "An error occurred - invalid number of images\n"
#define IMG_SUF_MSG "Enter images suffix:\n"
#define IMG_NUM_BINS_MSG "Enter number of bins:\n"
#define IMG_INVALID_NUM_BINS_MSG "An error occurred - invalid number of bins\n"
#define IMG_NUM_FEATURES_MSG "Enter number of features:\n"
#define IMG_INVALID_NUM_FEATURES_MSG "An error occurred - invalid number of features\n"
#define ENTER_QUERY_MSG "Enter a query image or # to terminate:\n"
#define HASH_MARK '#'
#define RGB_RANGE 3
#define ALOC_FAIL_MSG "An error occurred - allocation failure\n"
#define alocCheck(x) if(x == NULL){printf(ALOC_FAIL_MSG);exit(EXIT_FAILURE);}
#define NEAREST_LOCAL "Nearest images using global descriptors:\n"
#define NEAREST_GLOBAL "Nearest images using local descriptors:\n"

using namespace std;

//contains the distance of a feature with its image index
struct fDst{
		double distance;
		int imgIdx;
	};

//contains the number of matches of a feature with an image and its index
struct hits{
		int hitsNo;
		int imgIdx;
	};

//compares two fDst structs
int compare(const void *a, const void *b);

//free 2D int array
void free2DInt(int** arr, int length);
//free 2D double array
void free2DDouble(double** arr, int length);
//free 3D int array
void free3DInt(int*** arr, int length3D, int* length2D);
//free 3D double array
void free3DDouble(double*** arr, int length3D,int* length2D);

// generates an array of image paths for section B - step 3
char** pathGenerator(char* lib, char* prefix, char* suffix, int numOfImgs);

//prints the 5 nearest images considering global features
void nearest5Global(int** queryHist,int*** histDB, int nImages, int nBins);

//prints the 5 nearest images considering local features
void nearest5Local(double** querySIFT,double*** siftDB, int nImages, int queryFeaturesNo, int* DBfeaturesNo);

#endif /* MAIN_AUX_H_ */
