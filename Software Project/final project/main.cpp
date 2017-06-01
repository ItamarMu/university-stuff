#define EXIT_CODE "<>"
#define LOG_BEGPROC "Initializing image processing class"
#define LOG_ENDPROC "Image processing class successfully initialized"
#define LOG_EXT_MODE_ON "Extraction Mode On"
#define LOG_EXT_MODE_OFF "Extraction Mode Off"
#define LOG_GUI_MODE_ON "minGUI on"
#define LOG_GUI_MODE_OFF "minGUI off"
#define LOG_TOO_MANY_IMGS "Number of images in config file does not match the actual number of images"
#define LOG_FEAT_FILE_MISS "Missing expected features file"
#define LOG_ALOC_FAIL "Memory allocation failure"
#define LOG_FEAT_IMG_NUM "Getting features from image number %d"
#define LOG_FEAT_IMG_NUM_ERR "Error on getting image path for image number %d"
#define LOG_SAVE_FEAT "Saved features file for image number %d"
#define LOG_SAVE_FEAT_FAIL "Failed while saving features file for image number %d"
#define LOG_READ_FEAT_SUCC "Success at reading features file for image number %d"
#define LOG_READ_FEAT_FAIL "Failed while reading features file for image number %d"
#define LOG_FEAT_DB_FAIL "Failed while initializing features data base"
#define LOG_CONFIG_SUCC "Configuration was set successfully"
#define LOG_KDARR_FAIL "Failed while creating KD array"
#define LOG_KDARR_SUCC "Created KD array successfully"
#define LOG_KDTREE_FAIL "Failed while creating KD tree"
#define LOG_KDTREE_SUCC "Created KD tree successfully"
#define LOG_NEAR_NEIGHB "Checking nearest neighbors for feature number %d"
#define LOG_KNN_ERR "The features queue is empty after finding nearest neighbors for feature number %d"
#define LOG_FOUND_NEIGHB "Found nearest neighbors for feature number %d"
#define LOG_INIT_QUEUE "Created queue for best features"
#define LOG_INV_QUEUE "Queue does not contain enough elements"
#define LOG_MISS_PCA "PCA File Not Found"
#define LOG_MATCHES "number of matches for image num %d: %d"
#define LOG_WARN_IMG_NUM "Image library contains a big number of images and the extraction process could be prolonged"
#define CONFIG_FAIL "ERROR: Failed while building configuration"
#define DEFAULT_CONFIG "spcbir.config"
#define IMG_PATH_REQUEST "Please enter an image path:"
#define INV_PATH_MSG "Error! invalid path\n"
#define EXIT_MSG "...Exiting\n"
#define BEST_CAND "Best candidates for - %s - are:\n"
#define BIG_LIB 100
#define INVALID_ARGC "Invalid command line : use -c <config_filename>\n"
#define CONFIG_FILE_OPEN_ERR1 "The default configuration file spcbir.config couldn’t be open\n"
#define CONFIG_FILE_OPEN_ERR2 "The configuration file %s couldn’t be open\n"
#define C_FLAG "-c"

/*
 * The following macro frees all vars in case of early termination.
 */
#define freeMem() spLoggerDestroy();spPointDestroy2DPointArray(allFeats,numOfImgs,featsPerImg);freePointArray(queryFeats,queryFeatsNo);\
freePointArray(featuresDB,totalFeatsNo);spKDArrayDestroy(featuresKDArr);spKDTreeDestroy(featuresKDTree);spListElementDestroy(element);\
spBPQueueDestroy(featuresQueue);spBPQueueDestroy(bestMatches);free(featsPerImg);free(simiImgs);free(searchHitsPerImg); spConfigDestroy(config);

#include <cstdlib>
#include "SPImageProc.h"

extern "C" {
#include "SPConfig.h"
#include "SPPreProc.h"
#include "SPKDArray.h"
#include "SPKDTree.h"
#include "SPPoint.h"
#include "SPBPriorityQueue.h"
#include "SPListElement.h"
}

using namespace sp;
int main(int argc, char** argv) {
	//variables declaration
	SP_CONFIG_MSG msg;
	SPConfig config;
	int numOfImgs = 0, queryFeatsNo = 0, numOfSimiImgs, totalFeatsNo = 0, maxHits = 0;
	SPBPQueue bestMatches = NULL;
	SPPoint* queryFeats = NULL, *featuresDB = NULL;
	SPPoint** allFeats = NULL;
	SPKDArray featuresKDArr = NULL;
	KDTreeNode featuresKDTree = NULL;
	SPListElement element = NULL;
	SPBPQueue featuresQueue = NULL;
	int* featsPerImg = NULL, *searchHitsPerImg = NULL, *simiImgs = NULL;
	bool extract, saveFeatFile;
	char* loggerFilename;
	char imagePath[MAX_STRING], query[MAX_STRING], logmsg[MAX_STRING];
	SP_LOGGER_LEVEL loggerLevel;
	FILE* queryImg;
	//init configuration struct
	if (argc == 1) {
		config = spConfigCreate(DEFAULT_CONFIG,&msg);
		if (msg == SP_CONFIG_CANNOT_OPEN_FILE) {
			printf(CONFIG_FILE_OPEN_ERR1);
			return(EXIT_FAILURE);
		}
	}
	else if (argc == 3 && strcmp(argv[1],C_FLAG) == 0) {
		config = spConfigCreate(argv[2],&msg);
		if (msg == SP_CONFIG_CANNOT_OPEN_FILE) {
			printf(CONFIG_FILE_OPEN_ERR2,argv[2]);
			return(EXIT_FAILURE);
		}
	}
	else {
		printf(INVALID_ARGC);
		return(EXIT_FAILURE);
	}
    //init logger
    loggerFilename = spConfigGetLoggerFilename(config);
    loggerLevel = spConfigGetLoggerLevel(config);
    spLoggerCreate(loggerFilename, loggerLevel);
    //choose between extraction mode and non-extraction mode
    //init image processing
    spLoggerPrintInfo(LOG_BEGPROC);
    extract = spConfigIsExtractionMode(config,&msg);
    if(!extract && !validatePCA(config)){ //TODO comment line for debug
    	spLoggerPrintError(LOG_MISS_PCA, __FILE__, __func__, __LINE__);
    	freeMem();
    	return EXIT_FAILURE;
    }
    ImageProc proc(config);
    spLoggerPrintInfo(LOG_ENDPROC);
    numOfImgs = spConfigGetNumOfImages(config,&msg);
    if(extract && numOfImgs > BIG_LIB) spLoggerPrintWarning(LOG_WARN_IMG_NUM,__FILE__,__func__,__LINE__);
    numOfSimiImgs = spConfigGetNumOfSimiImages(config,&msg);
    if (extract)
    	spLoggerPrintInfo(LOG_EXT_MODE_ON);
    else
    	spLoggerPrintInfo(LOG_EXT_MODE_OFF);
    bool minGUI = spConfigMinimalGui(config, &msg);
    if (minGUI)
    	spLoggerPrintInfo(LOG_GUI_MODE_ON);
    else
    	spLoggerPrintInfo(LOG_GUI_MODE_ON);
    allFeats = (SPPoint**)malloc(sizeof(SPPoint*)*numOfImgs);
    if (!allFeats) {
    	spLoggerPrintError(LOG_ALOC_FAIL, __FILE__, __func__, __LINE__);
    	freeMem();
		return(EXIT_FAILURE);
    }
    featuresQueue = spBPQueueCreate(spConfigGetspKNN(config, &msg));
    if (!featuresQueue) {
    	spLoggerPrintError(LOG_ALOC_FAIL, __FILE__, __func__, __LINE__);
    	freeMem();
		return(EXIT_FAILURE);
    }
    bestMatches = spBPQueueCreate(numOfSimiImgs);
    if (!bestMatches) {
        spLoggerPrintError(LOG_ALOC_FAIL, __FILE__, __func__, __LINE__);
        freeMem();
        return(EXIT_FAILURE);
    }
    featsPerImg = (int*)malloc(sizeof(int)*numOfImgs);
    if (featsPerImg == NULL) {
        spLoggerPrintError(LOG_ALOC_FAIL, __FILE__, __func__, __LINE__);
        freeMem();
        return(EXIT_FAILURE);
    }
    simiImgs = (int*)calloc(spConfigGetspKNN(config, &msg), sizeof(int));
    if (simiImgs == NULL) {
        spLoggerPrintError(LOG_ALOC_FAIL, __FILE__, __func__, __LINE__);
        freeMem();
        return(EXIT_FAILURE);
    }
    searchHitsPerImg = (int*)calloc(numOfImgs, sizeof(int));
    if (searchHitsPerImg == NULL) {
        spLoggerPrintError(LOG_ALOC_FAIL, __FILE__, __func__, __LINE__);
        freeMem();
        return(EXIT_FAILURE);
    }
    if (extract) {  //in extraction mode, the ImageProc API is used to extract features
                    //from image files into an SPPoint matrix, followed by saving the
                    //features in .feats files for later use.
    	if (!validateImageLibrary(config)) {
    		spLoggerPrintError(LOG_TOO_MANY_IMGS,__FILE__,__func__,__LINE__);
    		free(allFeats);
			allFeats = NULL;
			freeMem();
			return(EXIT_FAILURE);
    	}
        for (int i = 0; i < numOfImgs; i++) {
            msg = spConfigGetImagePath(imagePath, config, i);
            if (msg == SP_CONFIG_SUCCESS) {
            	sprintf(logmsg, LOG_FEAT_IMG_NUM, i);
            	spLoggerPrintInfo(logmsg);
            }
            else {
            	sprintf(logmsg, LOG_FEAT_IMG_NUM_ERR, i);
            	spLoggerPrintError(logmsg, __FILE__, __func__, __LINE__);
            	freeMem();
            	return(EXIT_FAILURE);
            }
            allFeats[i] = proc.getImageFeatures(imagePath, i, &featsPerImg[i]);
            saveFeatFile = saveFeaturesToFile(config, allFeats[i], i, featsPerImg[i]);
            if (saveFeatFile) {
            	sprintf(logmsg, LOG_SAVE_FEAT, i);
            	spLoggerPrintInfo(logmsg);
            }
            else {
            	sprintf(logmsg, LOG_SAVE_FEAT_FAIL, i);
            	spLoggerPrintInfo(logmsg);
            	freeMem();
            	return(EXIT_FAILURE);
            }
        }
    }
    else {    //in non-extraction mode, the features are extracted from .feats files
            //containing the features found in each image.
    	if (!validateFeatsFiles(config)) { //
    		spLoggerPrintError(LOG_FEAT_FILE_MISS, __FILE__, __func__, __LINE__);
    		free(allFeats);
    		allFeats = NULL;
    		freeMem();
    		return(EXIT_FAILURE);
    	}
        for (int i = 0; i < numOfImgs; i++) {
            allFeats[i] = readFeaturesFromFile(config, i, &featsPerImg[i]);
            if (allFeats[i] == NULL) {
            	sprintf(logmsg, LOG_READ_FEAT_FAIL, i);
            	spLoggerPrintError(logmsg,__FILE__,__func__,__LINE__);
            	freeMem();
            	return(EXIT_FAILURE);
            }
            else {
            	sprintf(logmsg, LOG_READ_FEAT_SUCC, i);
            	spLoggerPrintInfo(logmsg);
            }
        }
    }
    //calculate total number of database features
    for(int i=0; i<numOfImgs; i++){
        totalFeatsNo += featsPerImg[i];
    }
    //insert the image database features into a KDArray
    featuresDB = (SPPoint*)malloc(sizeof(*featuresDB) * totalFeatsNo);
    if (featuresDB == NULL) {
        spLoggerPrintError(LOG_ALOC_FAIL, __FILE__, __func__, __LINE__);
        freeMem();
        return(EXIT_FAILURE);
    }
    for(int i=0, idx=0; i<numOfImgs; i++){
        for(int j=0; j<featsPerImg[i] && idx<totalFeatsNo; j++){
            featuresDB[idx++] = spPointCopy(allFeats[i][j]);
            if (featuresDB[idx-1] == NULL) {
            	spLoggerPrintError(LOG_FEAT_DB_FAIL, __FILE__, __func__, __LINE__);
            	freeMem();
            	return(EXIT_FAILURE);
            }
        }
    }
    //free features 2D array
	spPointDestroy2DPointArray(allFeats, numOfImgs, featsPerImg);
	allFeats = NULL;
	//free featsPerImg 1D array
    free(featsPerImg);
    featsPerImg = NULL;
    //init KDArray using the feature database 1D array
    featuresKDArr = spKDArrayCreate(featuresDB, totalFeatsNo, spConfigGetPCADim(config, &msg));
    //free the feature database 1D array
    freePointArray(featuresDB, totalFeatsNo);
    featuresDB = NULL;
	//init KDTree using the feature database
    if (featuresKDArr == NULL) {
    	spLoggerPrintError(LOG_KDARR_FAIL, __FILE__, __func__, __LINE__);
    	freeMem();
    	return(EXIT_FAILURE);
    }
    else {
    	spLoggerPrintInfo(LOG_KDARR_SUCC);
    }
    featuresKDTree = spKDTreeCreate(featuresKDArr, spConfigGetSplitMethod(config));

    //free KDArray
    spKDArrayDestroy(featuresKDArr);
    featuresKDArr = NULL;
    if (featuresKDTree == NULL) {
        spLoggerPrintError(LOG_KDTREE_FAIL, __FILE__, __func__, __LINE__);
        freeMem();
        return(EXIT_FAILURE);
    }
    else {
        spLoggerPrintInfo(LOG_KDTREE_SUCC);
    }

    //search for similiar images
    while(true){
        puts(IMG_PATH_REQUEST);
        scanf("%s",query);
        if(strcmp(query,EXIT_CODE) == 0){
            printf(EXIT_MSG);
            break;
        }
        queryImg = fopen(query, "r");
        //check if path is valid
        if(!queryImg){
            printf(INV_PATH_MSG);
            continue;
        }
        fclose(queryImg);
        //get features from query image
        queryFeats = proc.getImageFeatures(query, 0, &queryFeatsNo);
        //search for similar images
        for(int i=0; i<queryFeatsNo; i++){
            sprintf(logmsg, LOG_NEAR_NEIGHB, i);
            spLoggerPrintInfo(logmsg);
            kNearestNeighbors(featuresKDTree, featuresQueue,queryFeats[i]);
            if (!featuresQueue) {
            	sprintf(logmsg,LOG_KNN_ERR, i);
            	spLoggerPrintError(logmsg,__FILE__,__func__,__LINE__);
            	freeMem();
            	return(EXIT_FAILURE);
            }
            else {
            	sprintf(logmsg,LOG_FOUND_NEIGHB, i);
            	spLoggerPrintInfo(logmsg);
            }
            for(int j=0; j<spConfigGetspKNN(config, &msg); j++){
                element = spBPQueuePeek(featuresQueue);
                if (element == NULL) {
                	spLoggerPrintError(LOG_INV_QUEUE,__FILE__,__func__,__LINE__);
                	freeMem();
                	return(EXIT_FAILURE);
                }
                searchHitsPerImg[spListElementGetIndex(element)]++;
                spBPQueueDequeue(featuresQueue);
                //free peeked element
                spListElementDestroy(element);
                element = NULL;
            }
            spBPQueueClear(featuresQueue);
        }
        //free query image features
        freePointArray(queryFeats, queryFeatsNo);
        queryFeats = NULL;
        //rank images by number of hits
        for(int i=0; i<numOfImgs; i++){
            if(searchHitsPerImg[i] > maxHits) maxHits = searchHitsPerImg[i];
        }

        for(int i=0; i<numOfImgs; i++){
            SPListElement rank = spListElementCreate(i,(double)(maxHits - searchHitsPerImg[i]));
            spBPQueueEnqueue(bestMatches, rank);
            //free rank element
            spListElementDestroy(rank);
            rank = NULL;
        }

        spLoggerPrintInfo(LOG_INIT_QUEUE);
        for(int i=0; i<numOfSimiImgs; i++){
        	SPListElement element = spBPQueuePeek(bestMatches);
			simiImgs[i] = spListElementGetIndex(element);
			spBPQueueDequeue(bestMatches);
			//free element
			spListElementDestroy(element);
			element = NULL;
        }
        spBPQueueClear(bestMatches);
        //show similar images
        if (minGUI) {
            for (int i = 0; i < numOfSimiImgs; i++) {
                spConfigGetImagePath(imagePath, config, simiImgs[i]);
                proc.showImage(imagePath);
            }
        }
        else {
            printf(BEST_CAND, query);
            for (int i = 0; i < numOfSimiImgs; i++) {
                spConfigGetImagePath(imagePath, config, simiImgs[i]);
                printf("%s\n",imagePath);
                sprintf(logmsg,LOG_MATCHES,simiImgs[i] , searchHitsPerImg[simiImgs[i]]);
                spLoggerPrintDebug(logmsg,__FILE__, __func__, __LINE__);
            }
        }
        free(searchHitsPerImg);
        searchHitsPerImg = (int*)calloc(numOfImgs, sizeof(int));
        if (!searchHitsPerImg) {
            spLoggerPrintError(LOG_ALOC_FAIL, __FILE__, __func__, __LINE__);
            freeMem();
            return(EXIT_FAILURE);
        }
    }
    //free remaining allocations
    freeMem();
    return EXIT_SUCCESS;
}
