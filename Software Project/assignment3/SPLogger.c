#include "SPLogger.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define INFO_TITLE "---INFO---"
#define DEBUG_TITLE "---DEBUG---"
#define WARNING_TITLE "---WARNING---"
#define ERROR_TITLE "---ERROR---"
#define MESSAGE_TITLE "- message: "
#define FILE_TITLE "- file: "
#define FUNCTION_TITLE "- function: "
#define LINE_TITLE "- line: "
#define STRING_SIZE 128
//File open mode
#define SP_LOGGER_OPEN_MODE "w"

// Global variable holding the logger
SPLogger logger = NULL;

struct sp_logger_t {
	FILE* outputChannel; //The logger file
	bool isStdOut; //Indicates if the logger is stdout
	SP_LOGGER_LEVEL level; //Indicates the level
};

SP_LOGGER_MSG spLoggerCreate(const char* filename, SP_LOGGER_LEVEL level) {
	if (logger != NULL) { //Already defined
		return SP_LOGGER_DEFINED;
	}
	logger = (SPLogger) malloc(sizeof(*logger));
	if (logger == NULL) { //Allocation failure
		return SP_LOGGER_OUT_OF_MEMORY;
	}
	logger->level = level; //Set the level of the logger
	if (filename == NULL) { //In case the filename is not set use stdout
		logger->outputChannel = stdout;
		logger->isStdOut = true;
	} else { //Otherwise open the file in write mode
		logger->outputChannel = fopen(filename, SP_LOGGER_OPEN_MODE);
		if (logger->outputChannel == NULL) { //Open failed
			free(logger);
			logger = NULL;
			return SP_LOGGER_CANNOT_OPEN_FILE;
		}
		logger->isStdOut = false;
	}
	return SP_LOGGER_SUCCESS;
}

void spLoggerDestroy() {
	if (!logger) {
		return;
	}
	if (!logger->isStdOut) {//Close file only if not stdout
		fclose(logger->outputChannel);
	}
	free(logger);//free allocation
	logger = NULL;
}

// -Used by the print functions to print a message according to the level given, and returns the right SP_LOGGER_MSG
SP_LOGGER_MSG printLog(const char* msg, const char* file,
		const char* function, const int line,SP_LOGGER_LEVEL level){
	int res = 0;
	char* typeTitle;
	char* msgTitle = MESSAGE_TITLE;
	char* lineTitle = LINE_TITLE;
	char* fileTitle = FILE_TITLE;
	char* functionTitle = FUNCTION_TITLE;
	if(logger == NULL){
			return SP_LOGGER_UNDIFINED;
	}
	switch(level){

		case SP_LOGGER_INFO_WARNING_ERROR_LEVEL:
			typeTitle = INFO_TITLE;
			res = fprintf(logger->outputChannel,"%s\n%s%s\n",typeTitle,msgTitle,msg);
			if(res<0){return SP_LOGGER_WRITE_FAIL;}
			return SP_LOGGER_SUCCESS;
			break;
		case SP_LOGGER_ERROR_LEVEL:
			typeTitle = ERROR_TITLE;
			break;
		case SP_LOGGER_WARNING_ERROR_LEVEL:
			typeTitle = WARNING_TITLE;
			break;
		case SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL:
			typeTitle = DEBUG_TITLE;
			break;
	}
	res = fprintf(logger->outputChannel,"%s\n%s%s\n%s%s\n%s%d\n%s%s\n",
						typeTitle,fileTitle,file,functionTitle,function,lineTitle,line,msgTitle,msg);
				if(res<0){return SP_LOGGER_WRITE_FAIL;}
	return SP_LOGGER_SUCCESS;
}

SP_LOGGER_MSG spLoggerPrintError(const char* msg, const char* file,
		const char* function, const int line){
	if(msg == NULL || file == NULL || function == NULL || line<0){
					return SP_LOGGER_INVAlID_ARGUMENT;
	}
	return printLog(msg, file,function, line, SP_LOGGER_ERROR_LEVEL);
}

SP_LOGGER_MSG spLoggerPrintWarning(const char* msg, const char* file,
		const char* function, const int line){
	if(msg == NULL || file == NULL || function == NULL || line<0){
						return SP_LOGGER_INVAlID_ARGUMENT;
	}
	if(logger->level == SP_LOGGER_ERROR_LEVEL){
		return SP_LOGGER_SUCCESS;
	}
	return printLog(msg, file,function, line, SP_LOGGER_WARNING_ERROR_LEVEL);
}

SP_LOGGER_MSG spLoggerPrintInfo(const char* msg){
	if(msg == NULL){
					return SP_LOGGER_INVAlID_ARGUMENT;
	}
	if(logger->level != SP_LOGGER_INFO_WARNING_ERROR_LEVEL &&
			logger->level != SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL){
		return SP_LOGGER_SUCCESS;
	}
	return printLog(msg, "","", 0, SP_LOGGER_INFO_WARNING_ERROR_LEVEL);
}

SP_LOGGER_MSG spLoggerPrintDebug(const char* msg, const char* file,
		const char* function, const int line){
	if(msg == NULL || file == NULL || function == NULL || line<0){
						return SP_LOGGER_INVAlID_ARGUMENT;
	}
	if(logger->level ==  SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL){
			return printLog(msg, file,function, line, SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL);
		}
		return SP_LOGGER_SUCCESS;
}

SP_LOGGER_MSG spLoggerPrintMsg(const char* msg){
	int res;
	if(logger == NULL){
		return SP_LOGGER_UNDIFINED;
	}
	if(msg == NULL){
		return SP_LOGGER_INVAlID_ARGUMENT;
	}
	res = fprintf(logger->outputChannel,"%s\n",msg);
	if(res < 0){return SP_LOGGER_WRITE_FAIL;}
	return SP_LOGGER_SUCCESS;
}
