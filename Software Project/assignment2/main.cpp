#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/features2d.hpp>
#include <vector>
#include "sp_image_proc_util.h"
#include "main_aux.h"

using namespace cv;

int main() {
		//variables deceleration
		char lib[MAXLENGTH];
		char pref[100];
		char suf[10];
		char query[MAXLENGTH];
		char** paths;
		int* DBfeaturesNo;
		int imgNum, binsN, maxSIFT, queryFeaturesNo;
		int** queryHist;
		int*** histDB;
		double** querySIFT;
		double*** siftDB;

		printf(IMG_PATH_MSG);
		scanf("%s",lib);
		printf(IMG_PREF_MSG);
		scanf("%s",pref);
		printf(IMG_NUM_MSG);
		scanf("%d",&imgNum);
		if (imgNum < 1) {
			printf(IMG_INVALID_NUM_MSG);
			return EXIT_FAILURE;
		}
		printf(IMG_SUF_MSG);
		scanf("%s",suf);
		printf(IMG_NUM_BINS_MSG);
		scanf("%d",&binsN);
		if (binsN < 1) {
			printf(IMG_INVALID_NUM_BINS_MSG);
			return EXIT_FAILURE;
		}
		printf(IMG_NUM_FEATURES_MSG);
		scanf("%d",&maxSIFT);
		if (maxSIFT<1) {
			printf(IMG_INVALID_NUM_FEATURES_MSG);
			return EXIT_FAILURE;
		}

		paths = pathGenerator(lib,pref,suf,imgNum);

		// create SIFT and Hist databases:
		DBfeaturesNo = (int*)malloc(imgNum * sizeof(int));
		alocCheck(DBfeaturesNo);
		histDB = (int***)malloc(imgNum * sizeof(int**));
		alocCheck(histDB);
		for(int i=0; i<imgNum; i++){
			histDB[i] = spGetRGBHist(paths[i],binsN);
		}
		siftDB = (double***)malloc(imgNum * sizeof(double**));
		alocCheck(siftDB);
		for(int i=0; i<imgNum; i++){
			siftDB[i] = spGetSiftDescriptors(paths[i], maxSIFT, &DBfeaturesNo[i]);
		}
		//primary section - finding the nearest images to the query image
		while(true){
			printf(ENTER_QUERY_MSG);
			scanf("%s",query);
			if(query[0] == HASH_MARK){
				printf(EXIT_MSG);
				return EXIT_FAILURE;
			}
			queryHist = spGetRGBHist(query,binsN);
			nearest5Global(queryHist, histDB, imgNum, binsN);
			free2DInt(queryHist,RGB_RANGE);
			querySIFT = spGetSiftDescriptors(query, maxSIFT, &queryFeaturesNo);
			nearest5Local(querySIFT,siftDB, imgNum, queryFeaturesNo, DBfeaturesNo);
			free2DDouble(querySIFT,queryFeaturesNo);
		}
		return EXIT_SUCCESS;
}
