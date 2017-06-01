#include "SPPreProc.h"
#define LOG_INVDIM "The PCADIM in the configuration file does not match the feature dimension of the %i .feats file"
#define LOG_FEATNUM "The number of features in configuration file does is less than the declared number of features"
#define LOG_LESSFEAT "The number of features in configuration file is smaller than the actual number of features"
#define LOG_MOREFEAT "The number of features in configuration file is greater than the actual number of features"

bool saveFeaturesToFile(SPConfig config, SPPoint* features, int imgIdx, int numOfFeatures){
	FILE* file;
	SP_CONFIG_MSG msg;
	char path[MAX_STRING];
	int dim = spConfigGetPCADim(config, &msg);
	spConfigGetFeaturesPath(path, config, imgIdx);
	file = fopen(path, "w");
	if(fprintf(file,"%d\n",imgIdx) < 0){
		fclose(file);
		return false;
	}
	if(fprintf(file,"%d\n",numOfFeatures) < 0){
		fclose(file);
		return false;
	}
	if(fprintf(file,"%d\n",dim) < 0){
		fclose(file);
		return false;
	}
	for(int i=0;i<numOfFeatures;i++){
		for(int j=0;j<dim;j++){
			if(fprintf(file, "%f ", spPointGetAxisCoor(features[i], j)) < 0){
				fclose(file);
				return false;
			}
		}
		if( fprintf(file, "\n") < 0){
			fclose(file);
			return false;
		}
	}
	fclose(file);
	return true;
}

SPPoint* readFeaturesFromFile(SPConfig config, int idx, int* numOfFeats){
	SP_CONFIG_MSG msg;
	char featPath[MAX_STRING], line[MAX_STRING], token = ' ';
	char numInFeat[MAX_STRING]; //represents one number in a feature
	int NoOfFeats = 0, dim = 0;
	FILE* featFile;
	SPPoint* mat;
	spConfigGetFeaturesPath(featPath, config, idx);
	featFile = fopen(featPath,"r");
	if(!featFile) return NULL;
	fgets(line, MAX_STRING, featFile); //skip index line
	fgets(line, MAX_STRING, featFile);
	line[strcspn(line, "\n")] = '\0';
	NoOfFeats = atoi(line);
	*numOfFeats = NoOfFeats;
	fgets(line, MAX_STRING, featFile);
	line[strcspn(line, "\n")] = '\0';
	dim = atoi(line);

	//validating .feats file
	if(dim != spConfigGetPCADim(config, &msg)){
		char logMsg[MAX_STRING];
		sprintf(logMsg, LOG_INVDIM, idx);
		spLoggerPrintError(logMsg, __FILE__, __func__, __LINE__);
		return NULL;
	}

	mat = (SPPoint*)malloc(sizeof(SPPoint)*NoOfFeats);
	if (!mat) {
		return NULL;
	}
	double featLine[dim];
	for (int i = 0; i < NoOfFeats; i++) {
		if(!fgets(line, MAX_STRING, featFile)){
			spLoggerPrintError(LOG_LESSFEAT, __FILE__, __func__, __LINE__);
			return NULL;
		}
		line[strcspn(line, "\n")] = '\0';
		strcpy(numInFeat, strtok(line, &token));
		featLine[0] = atof(numInFeat);
		for (int j = 1; j < dim; j++) {
			strcpy(numInFeat, strtok(NULL, &token));
			featLine[j] = atof(numInFeat);
		}
		mat[i] = spPointCreate(featLine, dim, idx);
	}
	if(fgets(line, MAX_STRING, featFile)){ // if there are extra features
		spLoggerPrintError(LOG_MOREFEAT, __FILE__, __func__, __LINE__);
		return NULL;
	}
	fclose(featFile);
	return mat;
}

bool validateFeatsFiles(SPConfig config){
	FILE* file;
	SP_CONFIG_MSG msg;
	char path[MAX_STRING];
	int numOfImgs = spConfigGetNumOfImages(config, &msg);
	for(int i=0; i<numOfImgs; i++){
		spConfigGetFeaturesPath(path, config, i);
		file = fopen(path, "r");
		if(!file){
			return false;
		}
		fclose(file);
	}
	return true;
}

bool validateImageLibrary(SPConfig config){
	FILE* file;
	SP_CONFIG_MSG msg;
	char path[MAX_STRING];
	int numOfImgs = spConfigGetNumOfImages(config, &msg);
	for(int i=0; i<numOfImgs; i++){
		spConfigGetImagePath(path, config, i);
		file = fopen(path, "r");
		if(!file){
			return false;
		}
		fclose(file);
	}
	return true;
}

bool validatePCA(SPConfig config){
	FILE* file;
	char path[MAX_STRING];
	spConfigGetPCAPath(path, config);
	file = fopen(path, "r");
	if(!file){
		return false;
	}
	fclose(file);
	return true;
}
