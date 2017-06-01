#include "../SPPoint.h"
#include "unit_test_util.h"
#include <stdbool.h>

//Checks if copy Works
bool pointBasicCopyTest() {
	double data[2] = { 1.0, 1.0 };
	int dim = 2;
	int index = 1;
	SPPoint p = spPointCreate(data, dim, index);
	SPPoint q = spPointCopy(p);
	ASSERT_TRUE(spPointGetIndex(p) == spPointGetIndex(q));
	ASSERT_TRUE(spPointGetDimension(p) == spPointGetDimension(q));
	for (int i = 0; i < spPointGetDimension(p); i++) {
		ASSERT_TRUE(spPointGetAxisCoor(p, i) == spPointGetAxisCoor(q, i));
	}
	spPointDestroy(p);
	spPointDestroy(q);
	return true;
}
//Checks L2Distance
bool pointBasicL2Distance() {
	double data1[2] = { 1.0, 1.0 };
	double data2[2] = { 1.0, 0.0 };
	int dim1 = 2;
	int dim2 = 2;
	int index1 = 1;
	int index2 = 1;
	SPPoint p = spPointCreate((double *)data1, dim1, index1);
	SPPoint q = spPointCreate((double *)data2, dim2, index2);
	ASSERT_TRUE(spPointL2SquaredDistance(p,p) == 0.0);
	ASSERT_TRUE(spPointL2SquaredDistance(q,q) == 0.0);
	ASSERT_FALSE(spPointL2SquaredDistance(p,q) == 0.0);
	spPointDestroy(p);
	spPointDestroy(q);
	return true;
}
//Checks that L2Distance is even between the pairs (1,1)-(2,2) and (2,2)-(3,3)
bool pointL2Dcheck() {
	double data1[2] = { 1.0, 1.0 };
	double data2[2] = { 2.0, 2.0 };
	double data3[2] = { 3.0, 3.0 };
	int dim1 = 2, dim2 = 2, dim3 = 2;
	int index = 1;
	SPPoint p1 = spPointCreate(data1,dim1,index);
	SPPoint p2 = spPointCreate(data2,dim2,index);
	SPPoint p3 = spPointCreate(data3,dim3,index);
	ASSERT_TRUE(spPointL2SquaredDistance(p1,p2) == spPointL2SquaredDistance(p2,p3));
	spPointDestroy(p1);spPointDestroy(p2);spPointDestroy(p3);
	return true;
}
//Check getters
bool getterCheck() {
	double data[2] = { 1.0, 1.0 };
	int dim = 2;
	int index = 1;
	SPPoint p = spPointCreate(data, dim, index);
	ASSERT_TRUE(spPointGetIndex(p) == index);
	ASSERT_TRUE(spPointGetDimension(p) == dim);
	for (int i = 0; i < spPointGetDimension(p); i++) {
		ASSERT_TRUE(spPointGetAxisCoor(p, i) == data[i]);
	}
	spPointDestroy(p);
	return true;
}
int main() {
	RUN_TEST(pointBasicCopyTest);
	RUN_TEST(pointBasicL2Distance);
	RUN_TEST(pointL2Dcheck);
	RUN_TEST(getterCheck);
	return 0;
}
