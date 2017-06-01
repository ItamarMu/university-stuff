#include "SPPoint.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>

struct sp_point_t{
	double* data;
	int dim;
	int index;
};

SPPoint spPointCreate(double* data, int dim, int index) {
	SPPoint res;
	res = (SPPoint) malloc(sizeof(*res));
	res->data = (double*) malloc(sizeof(double)*dim);
	if (res == NULL || data == NULL || dim <= 0 || index < 0 || res->data == NULL) {
		free(res);
		return NULL;
	}
	for (int i=0; i<dim; i++) {
		res->data[i] = data[i];
	}
	res->dim = dim;
	res->index = index;
	return res;
}

SPPoint spPointCopy(SPPoint source){
	assert (source != NULL);
	SPPoint res = spPointCreate(source->data, source->dim, source->index);
	if(res == NULL){
		return NULL;
	}
	return res;
}

void spPointDestroy(SPPoint point) {
	if (point == NULL) {
		return;
	}
	free(point->data);
	free(point);
	return;
}

int spPointGetDimension(SPPoint point){
	assert(point != NULL);
	return point->dim;
}

int spPointGetIndex(SPPoint point) {
	assert(point != NULL);
	return point->index;
}

double spPointGetAxisCoor(SPPoint point, int axis){
	assert(point!=NULL && axis < point->dim && axis>=0);
	return point->data[axis];
}

/**
 * Calculates the L2-squared distance between p and q.
 * The L2-squared distance is defined as:
 * (p_1 - q_1)^2 + (p_2 - q_1)^2 + ... + (p_dim - q_dim)^2
 *
 * @param p - The first point
 * @param q - The second point
 * @assert p!=NULL AND q!=NULL AND dim(p) == dim(q)
 * @return
 * The L2-Squared distance between p and q
 */
double spPointL2SquaredDistance(SPPoint p, SPPoint q){
	assert(p!=NULL && q!=NULL && p->dim == q->dim);
	double distance = 0;

	for(int i=0; i< p->dim; i++){
		double sub = p->data[i] - q->data[i];
		distance += sub*sub;
	}
	return distance;
}
