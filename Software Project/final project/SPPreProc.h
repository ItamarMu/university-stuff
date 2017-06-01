#ifndef SPPREPROC_H_
#include "SPPoint.h"
#include "SPConfig.h"

#define SPPREPROC_H_

/*
 * Returns an array of SPPoints which represents the features of an image from the directory in the config file
 * with the right index given by the user, also changes numOfFeats to contain the correct number of features
 * for this image.
 *
 * @param config - the configuration structure
 * @assert config != NULL
 * @param idx - the image index
 * @param numOfFeats - pointer in which the number of features is stored
 * @assert numOfFeats != NULL
 * @return array of SPPoints which represent features and change &numOfFeats to contain the number of features
 * or NULL if the mehtod fails.
 *
 */
SPPoint* readFeaturesFromFile(SPConfig config, int idx, int* numOfFeats);
/*
 * Saves features of a given image in a file.
 *
 * @param config - the configuration file
 * @param features - the features array
 * @param imgIdx - the image's index whiting the image directory
 * @param numOfFeatures - the number of features of the given image
 *
 * @pre - parameters are valid
 * */
bool saveFeaturesToFile(SPConfig config, SPPoint* features, int imgIdx, int numOfFeatures);

/*
 * Validates all .feats files exist
 *
 * @param config - the configuration file
 *
 * @pre - parameters are valid
 * */
bool validateFeatsFiles(SPConfig config);

/*
 * Validates all image files exist
 *
 * @param config - the configuration file
 *
 * @pre - parameters are valid
 * */
bool validateImageLibrary(SPConfig config);

bool validatePCA(SPConfig config);


#endif /* SPPREPROC_H_ */
