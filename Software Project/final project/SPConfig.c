#include "SPConfig.h"

#define INVALID_CONF_LINE_ERROR "File: %s\nLine: %d\nMessage: Invalid configuration line\n"
#define INVALID_VALUE_ERROR "File: %s\nLine: %d\nMessage: Invalid value - constraint not met\n"
#define PARAMETER_NOT_SET_ERROR "File: %s\nLine: %d\nMessage: Parameter %s is not set\n"
#define SP_IMG_DIR "spImagesDirectory"
#define SP_IMG_PRE "spImagesPrefix"
#define SP_IMG_SUF "spImagesSuffix"
#define SP_NUM_IMGS "spNumOfImages"
#define SP_PCAD_DIM "spPCADimension"
#define SP_PCA_FILE "spPCAFilename"
#define SP_NUM_FEATS "spNumOfFeatures"
#define SP_EXT_MODE "spExtractionMode"
#define SP_NUM_SIMIMGS "spNumOfSimilarImages"
#define SP_KD_SPLIT "spKDTreeSplitMethod"
#define SP_KD_RANDOM "RANDOM"
#define SP_KD_MAX_SPREAD "MAX_SPREAD"
#define SP_KD_INC "INCREMENTAL"
#define SPKNN "spKNN"
#define TRUE "true"
#define FALSE "false"
#define SP_MIN_GUI "spMinimalGUI"
#define SP_LOG_LEV "spLoggerLevel"
#define SP_LOG_FILE "spLoggerFilename"
#define JPG ".jpg"
#define PNG ".png"
#define BMP ".bmp"
#define GIF ".gif"
#define DEFAULT_SPPCADIMENSION 20
#define DEFAULT_SPPCAFILENAME "pca.yml"
#define DEFAULT_SPNUMOFFEATURES 100
#define DEFAULT_SPEXTRACTIONMODE true
#define DEFAULT_SPMINIMALGUI false
#define DEFAULT_SPNUMOFSIMILARIMAGES 1
#define DEFAULT_SPKNN 1
#define DEFAULT_SPKDTREESPLITMETHOD MAX_SPREAD
#define DEFAULT_SPLOGGERLEVEL 3
#define DEFAULT_SPLOGGERFILENAME "stdout"
#define destroyAndReturn() {\
						spConfigDestroy(config);\
						fclose(configFile);\
						return NULL;\
						}\

struct sp_config_t{
	char* spImagesDirectory;
	char* spImagesPrefix;
	char* spImagesSuffix;
	int spNumOfImages;
	int spPCADimension;
	char* spPCAFilename;
	int spNumOfFeatures;
	bool spExtractionMode;
	int spNumOfSimilarImages;
	KDT_SPLIT_METHOD spKDTreeSplitMethod;
	int spKNN;
	bool spMinimalGUI;
	int spLoggerLevel;
	char* spLoggerFilename;
};

/*
 * Returns true if the given string represents a valid integer.
 *
 * @param str
 * @param msg - pointer in which the msg returned by the function is stored.
 */
bool isInt (char* str) { //checks if the given string represents a valid integer
	if(str == NULL || str[0] == '\0'){
		return false;
	}
	if ((str[0] == '+' || str[0] == '-') && isdigit(str[1])) {
		for (int i = 2; i<MAX_STRING; i++) {
			if (str[i] == '\0')
				return true;
			if (!isdigit(str[i]))
				return false;
		}
	}
	if (isdigit(str[0])) {
		for (int i = 1; i<MAX_STRING; i++) {
			if (str[i] == '\0')
				return true;
			if (!isdigit(str[i]))
				return false;
		}
	}
	return false;
}

/**
 * Interprets var as one of the fields in the configuration struct
 * and val as its corresponding assignment value.
 * If var represents an existing field and val represents a legal assigment,
 * the function will assign val into the correct field in the struct
 * If no existing field was matched, an error message will be returned
 *
 * @param config - the configuration struct
 * @assert config != NULL
 * @param var
 * @param val
 * @return NULL in case an error occurs. Otherwise, a pointer to a struct which
 * 		   contains all system configuration.
 *
* @return
 * - SP_CONFIG_INVALID_STRING - if val is empty or var doesn't represent
 *   one of the fields in the configuration struct
 *   or val doesn't meet the string requirements for the corresponding field.
 * - SP_CONFIG_INVALID_INTEGER - if val doesn't meet the integer requirements for the corresponding field.
 * - SP_CONFIG_SUCCESS - in case of success
 */
SP_CONFIG_MSG spSetValToVar(const SPConfig config, char* var, char* val) {
	int valtoint;
	if (val[0] == '\0' || var[0] == '\0'){
		return SP_CONFIG_INVALID_STRING;
	}
	if (strcmp(var,SP_IMG_DIR) == 0) { //SP_CONFIG_INVALID_STRING @ SP_CONFIG_INVALID_INTEGER
		strcpy(config->spImagesDirectory,val);
		return SP_CONFIG_SUCCESS;
	}
	if (strcmp(var,SP_IMG_PRE) == 0) {
		strcpy(config->spImagesPrefix,val);
		return SP_CONFIG_SUCCESS;
	}
	if (strcmp(var,SP_IMG_SUF) == 0) {
		if (strcmp(val,JPG) != 0 && strcmp(val,PNG) != 0 && strcmp(val,BMP) != 0
				&& strcmp(val,GIF) != 0) {
			return SP_CONFIG_INVALID_STRING;
		}
		strcpy(config->spImagesSuffix,val);
		return SP_CONFIG_SUCCESS;
	}
	if (strcmp(var,SP_NUM_IMGS) == 0) {
		if (!isInt(val)) {
						return SP_CONFIG_INVALID_STRING;
		}
		valtoint = atoi(val);
		if (valtoint <= 0) {
						return SP_CONFIG_INVALID_INTEGER;
		}
		config->spNumOfImages = valtoint;
		return SP_CONFIG_SUCCESS;
	}
	if (strcmp(var,SP_PCAD_DIM) == 0) {
		if (!isInt(val)) {
						return SP_CONFIG_INVALID_STRING;
			}
		valtoint = atoi(val);
		if (valtoint < 10 || valtoint > 28) {
			return SP_CONFIG_INVALID_INTEGER;
		}
		config->spPCADimension = valtoint;
		return SP_CONFIG_SUCCESS;
	}
	if (strcmp(var,SP_PCA_FILE) == 0) {
		strcpy(config->spPCAFilename,val);
		return SP_CONFIG_SUCCESS;
	}
	if (strcmp(var,SP_NUM_FEATS) == 0) {
		if (!isInt(val)) {
					return SP_CONFIG_INVALID_STRING;
			}
		valtoint = atoi(val);
		if (valtoint <= 0) {
			return SP_CONFIG_INVALID_INTEGER;
		}
		config->spNumOfFeatures = valtoint;
		return SP_CONFIG_SUCCESS;
	}
	if (strcmp(var,SP_EXT_MODE) == 0) {
		if (strcmp(val,TRUE) == 0) {
			config->spExtractionMode = true;
			return SP_CONFIG_SUCCESS;
		}
		if (strcmp(val,FALSE) == 0) {
			config->spExtractionMode = false;
			return SP_CONFIG_SUCCESS;
		}
		return SP_CONFIG_INVALID_STRING;
	}
	if (strcmp(var,SP_NUM_SIMIMGS) == 0) {
		if (!isInt(val)) {
					return SP_CONFIG_INVALID_STRING;
			}
		valtoint = atoi(val);
		if (valtoint <= 0) {
			return SP_CONFIG_INVALID_INTEGER;
		}
		config->spNumOfSimilarImages = valtoint;
		return SP_CONFIG_SUCCESS;
	}
	if (strcmp(var,SP_KD_SPLIT) == 0) {
		if (strcmp(val,SP_KD_RANDOM) == 0) {
			config->spKDTreeSplitMethod = RANDOM;
			return SP_CONFIG_SUCCESS;
		}
		if (strcmp(val,SP_KD_MAX_SPREAD) == 0) {
			config->spKDTreeSplitMethod = MAX_SPREAD;
			return SP_CONFIG_SUCCESS;
		}
		if (strcmp(val,SP_KD_INC) == 0) {
			config->spKDTreeSplitMethod = INCREMENTAL;
			return SP_CONFIG_SUCCESS;
		}
		return SP_CONFIG_INVALID_STRING;
	}
	if (strcmp(var,SPKNN) == 0) {
		if (!isInt(val)) {
			return SP_CONFIG_INVALID_STRING;
		}
		valtoint = atoi(val);
		if (valtoint <= 0) {
			return SP_CONFIG_INVALID_INTEGER;
		}
		config->spKNN = valtoint;
		return SP_CONFIG_SUCCESS;
	}
	if (strcmp(var,SP_MIN_GUI) == 0) {
		if (strcmp(val,TRUE) == 0) {
			config->spMinimalGUI = true;
			return SP_CONFIG_SUCCESS;
		}
		if (strcmp(val,FALSE) == 0) {
			config->spMinimalGUI = false;
			return SP_CONFIG_SUCCESS;
		}
		return SP_CONFIG_INVALID_STRING;
	}
	if (strcmp(var,SP_LOG_LEV) == 0) {
		if (!isInt(val)) {
			return SP_CONFIG_INVALID_STRING;
		}
		valtoint = atoi(val);
		if (valtoint > 4 || valtoint < 1) {
			return SP_CONFIG_INVALID_INTEGER;
		}
		config->spLoggerLevel = valtoint;
		return SP_CONFIG_SUCCESS;
	}
	if (strcmp(var,SP_LOG_FILE) == 0) {
		strcpy(config->spLoggerFilename, val);
		return SP_CONFIG_SUCCESS;
	}
	return SP_CONFIG_INVALID_STRING;
}

/**
 * Sets the configuration sturct's fields to their corresponding default values.
 *
 * @param config - the configuration structure
 */
void setDefaults(SPConfig config){
	config->spPCADimension = DEFAULT_SPPCADIMENSION;
	config->spPCAFilename = (char*)malloc(sizeof(char) * MAX_STRING);
	strcpy(config->spPCAFilename, DEFAULT_SPPCAFILENAME);
	config->spNumOfFeatures = DEFAULT_SPNUMOFFEATURES;
	config->spExtractionMode = DEFAULT_SPEXTRACTIONMODE;
	config->spMinimalGUI = DEFAULT_SPMINIMALGUI;
	config->spNumOfSimilarImages = DEFAULT_SPNUMOFSIMILARIMAGES;
	config->spKNN = DEFAULT_SPKNN;
	config->spKDTreeSplitMethod = DEFAULT_SPKDTREESPLITMETHOD;
	config->spLoggerLevel = DEFAULT_SPLOGGERLEVEL;
	config->spLoggerFilename = (char*)malloc(sizeof(char) * MAX_STRING);
	config->spImagesDirectory = (char*)malloc(sizeof(char) * MAX_STRING);
	config->spImagesPrefix = (char*)malloc(sizeof(char) * MAX_STRING);
	config->spImagesSuffix = (char*)malloc(sizeof(char) * MAX_STRING);
	strcpy(config->spLoggerFilename, DEFAULT_SPLOGGERFILENAME);
}

/**
 * Returns true if the string is compounded entirely by whitespace characters.
 *
 * @param str
 * @param config - the configuration structure
 * @param index - the index of the image.
 *
 * @return
 * - SP_CONFIG_INVALID_ARGUMENT - if imagePath == NULL or config == NULL
 * - SP_CONFIG_INDEX_OUT_OF_RANGE - if index >= spNumOfImages
 * - SP_CONFIG_SUCCESS - in case of success
 */
bool isWhiteSpaceString(char* str){
	if(str == NULL || str[0] == '\0'){
		return false;
	}
	for(int i=0; i<(int)strlen(str) && str[i] != '\0'; i++){
		if(!isspace(str[i])){
			return false;
		}
	}
	return true;
}

/**
 * Cleans spaces at the end and at the beginning of the given string and returns true if there are
 * no spaces in what's left, and false otherwise
 *
 * @param str
 *
 * @return
 * - True if there are no spaces left after clearing the from the sides and false otherwise,
 * also changes the string to have no spaces from both its sides.
 */
bool isValidStr (char** str) {
	int i = 0, t = 0, r = 0;
	if(str == NULL || str[0][0] == '\0'){
		return false;
	}
	while (isspace(str[0][i])) {
		i++;
	}
	*str = *str+i;
	while (str[0][t] != '\0') {
		t++;
	}
	t = t-1;
	while (t >= 0 && isspace(str[0][t])) {
		t = t-1;
	}
	str[0][t+1] = '\0';
	while (str[0][r] != '\0') {
		if (isspace(str[0][r]))
			return false;
		r++;
	}
	return true;
}

/**
 * Returns true if the string contains a single delimeter character
 *
 * @param str
 * @param del - the delimeter character to be searched.
 *
 * @return
 * - True if there is a single del chararcter in the string.
 */
bool isSingleDelimeter(const char* str, const char del){
	int count = 0;
	if(str == NULL || str[0] == '\0'){
		return false;
	}
	for(int i=0;i<(int)strlen(str) && str[i] != '\0'; i++){
		if(str[i] == del){
			count++;
		}
	}
	if(count == 1){
		return true;
	}
	return false;
}

SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg) {
	//variable declaration
	FILE* configFile;
	char line[MAX_STRING], var[MAX_STRING];
	int lineNo = 0;
	SPConfig config;
	char token;
	char* leftPtr;
	char* rightPtr;
	//
	if(!filename){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return NULL;
	}
	configFile = fopen(filename,"r");
	if(!configFile){
		*msg = SP_CONFIG_CANNOT_OPEN_FILE;
		return NULL;
	}
	config = (SPConfig)malloc(sizeof(*config));
	setDefaults(config);
	if(!config){
		*msg = SP_CONFIG_ALLOC_FAIL;
		return NULL;
	}
	while((fgets(line, MAX_STRING, configFile))){ //file parsing
		lineNo++;
		sscanf(line,"%s",var);
		if(var[0] == '#' || isWhiteSpaceString(line)){
			continue;
		}
		token = '=';
		if(isSingleDelimeter(line, token)){ // the second condition verifies there is no "garbage" after the 3rd string
			char left[MAX_STRING];
			strcpy(left, strtok(line, &token));
			char right[MAX_STRING];
			strcpy(right, strtok(NULL, &token));
			leftPtr = left;
			rightPtr = right;
			right[strcspn(right, "\n")] = '\0';
			if(!isValidStr(&leftPtr) || !isValidStr(&rightPtr)){
				printf(INVALID_CONF_LINE_ERROR, filename, lineNo);
				*msg = SP_CONFIG_INVALID_STRING;
				destroyAndReturn();
			}
			SP_CONFIG_MSG result = spSetValToVar(config, leftPtr, rightPtr);
			if(result != SP_CONFIG_SUCCESS){
				printf(INVALID_VALUE_ERROR, filename, lineNo);
				*msg = result;
				destroyAndReturn();
			}
		} else {
			printf(INVALID_CONF_LINE_ERROR, filename, lineNo);
			*msg = SP_CONFIG_INVALID_STRING;
			destroyAndReturn();
		}
	}

	if(!config->spImagesDirectory){
		*msg = SP_CONFIG_MISSING_DIR;
		destroyAndReturn();
	}
	if(!config->spImagesPrefix){
		*msg = SP_CONFIG_MISSING_PREFIX;
		destroyAndReturn();
	}
	if(!config->spImagesSuffix){
		*msg = SP_CONFIG_MISSING_SUFFIX;
		destroyAndReturn();
	}
	if(!config->spNumOfImages){
		*msg = SP_CONFIG_MISSING_NUM_IMAGES;
		destroyAndReturn();
	}
	fclose(configFile);
	return config;
}

bool spConfigIsExtractionMode(const SPConfig config, SP_CONFIG_MSG* msg) {
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return false;
	}
	*msg = SP_CONFIG_SUCCESS;
	if (config->spExtractionMode) {
		return true;
	}
	return false;
}

int spConfigGetNumOfFeatures(const SPConfig config, SP_CONFIG_MSG* msg) {
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spNumOfFeatures;
}

int spConfigGetNumOfImages(const SPConfig config, SP_CONFIG_MSG* msg) {
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spNumOfImages;
}

int spConfigGetPCADim(const SPConfig config, SP_CONFIG_MSG* msg) {
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spPCADimension;
}

SP_CONFIG_MSG spConfigGetImagePath(char* imagePath, const SPConfig config,
		int index) {
	if (imagePath == NULL || config == NULL) {
		return SP_CONFIG_INVALID_ARGUMENT;
	}
	if (index >= config->spNumOfImages) {
		return SP_CONFIG_INDEX_OUT_OF_RANGE;
	}
	sprintf(imagePath,"%s%s%d%s",config->spImagesDirectory,config->spImagesPrefix,index,config->spImagesSuffix);
	return SP_CONFIG_SUCCESS;
}

SP_CONFIG_MSG spConfigGetPCAPath(char* pcaPath, const SPConfig config) {
	if(pcaPath == NULL || config == NULL){
		return SP_CONFIG_INVALID_ARGUMENT;
	}
	sprintf(pcaPath,"%s%s",config->spImagesDirectory,config->spPCAFilename);
	return SP_CONFIG_SUCCESS;
}

bool spConfigMinimalGui(const SPConfig config, SP_CONFIG_MSG* msg){
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return false;
	}
	*msg = SP_CONFIG_SUCCESS;
	if (config->spMinimalGUI) {
		return true;
	}
	return false;
}

void spConfigDestroy(SPConfig config) {
	if(config){
		free(config->spImagesDirectory);
		free(config->spImagesPrefix);
		free(config->spImagesSuffix);
		free(config->spPCAFilename);
		free(config->spLoggerFilename);
		free(config);
	}
}

SP_CONFIG_MSG spConfigGetFeaturesPath(char* imagePath, const SPConfig config,
		int index) {
	if (imagePath == NULL || config == NULL) {
		return SP_CONFIG_INVALID_ARGUMENT;
	}
	if (index >= config->spNumOfImages) {
		return SP_CONFIG_INDEX_OUT_OF_RANGE;
	}
	sprintf(imagePath,"%s%s%d.feats",config->spImagesDirectory,config->spImagesPrefix,index);
	return SP_CONFIG_SUCCESS;
}

int spConfigGetNumOfSimiImages(const SPConfig config, SP_CONFIG_MSG* msg) {
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spNumOfSimilarImages;
}

int spConfigGetspKNN(const SPConfig config, SP_CONFIG_MSG* msg) {
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spKNN;
}

KDT_SPLIT_METHOD spConfigGetSplitMethod(const SPConfig config){
	if(!config) return -1;
	return config->spKDTreeSplitMethod;
}

char* spConfigGetLoggerFilename(const SPConfig config){
	if(config->spLoggerFilename){
		if (strcmp(config->spLoggerFilename, "stdout") == 0) return NULL;
		return config->spLoggerFilename;
	}
	return NULL;
}

SP_LOGGER_LEVEL spConfigGetLoggerLevel(const SPConfig config){
	switch(config->spLoggerLevel){
	case 1:
		return SP_LOGGER_ERROR_LEVEL;
	case 2:
		return SP_LOGGER_WARNING_ERROR_LEVEL;
	case 3:
		return SP_LOGGER_INFO_WARNING_ERROR_LEVEL;
	default:
		return SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL;
	}
}

