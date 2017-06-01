#include "unit_test_util.h"
#include "../SPListElement.h"
#include "../SPBPriorityQueue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

//Checks create function
static bool qCreate() {
	SPBPQueue q = spBPQueueCreate(5);
	ASSERT_TRUE(q!=NULL);
	spBPQueueDestroy(q);
	return true;
}
//Checks invalid input case
static bool qNull(){
	SPBPQueue q = spBPQueueCreate(0);
	ASSERT_TRUE(q==NULL);
	spBPQueueDestroy(q);
	return true;
}
//Checks empty function
static bool qEmpty() {
	SPBPQueue q = spBPQueueCreate(5);
	ASSERT_TRUE(spBPQueueIsEmpty(q));
	spBPQueueDestroy(q);
	return true;
}
//Checks max size function
static bool qMaxSize() {
	SPBPQueue q = spBPQueueCreate(5);
	ASSERT_TRUE(spBPQueueGetMaxSize(q) == 5);
	spBPQueueDestroy(q);
	return true;
}
//Checks size function
static bool qSize() {
	SPBPQueue q = spBPQueueCreate(5);
	SPListElement e1 = spListElementCreate(1, 4.0); spBPQueueEnqueue(q,e1); //Queue: {(1,4.0)}
	SPListElement e2 = spListElementCreate(2, 3.0); spBPQueueEnqueue(q,e2);//Queue: {(2,3.0),(1,4.0)}
	SPListElement e3 = spListElementCreate(3, 17.0); spBPQueueEnqueue(q,e3);//Queue: {(2,3.0),(1,4.0),(3,17.0)}
	SPListElement e4 = spListElementCreate(4, 2.0); spBPQueueEnqueue(q,e4); //Queue: {(4,2.0),(2,3.0),(1,4.0),(3,17.0)}
	ASSERT_TRUE(spBPQueueSize(q) == 4);
	spBPQueueDestroy(q);
	spListElementDestroy(e1);spListElementDestroy(e2);
	spListElementDestroy(e3);spListElementDestroy(e4);
	return true;
}
//Checks full function
static bool qFull() {
	SPBPQueue q = spBPQueueCreate(5);
	SPListElement e1 = spListElementCreate(1, 4.0); spBPQueueEnqueue(q,e1); //Queue: {(1,4.0)}
	SPListElement e2 = spListElementCreate(2, 3.0); spBPQueueEnqueue(q,e2);//Queue: {(2,3.0),(1,4.0)}
	SPListElement e3 = spListElementCreate(3, 17.0); spBPQueueEnqueue(q,e3);//Queue: {(2,3.0),(1,4.0),(3,17.0)}
	SPListElement e4 = spListElementCreate(4, 2.0); spBPQueueEnqueue(q,e4); //Queue: {(4,2.0),(2,3.0),(1,4.0),(3,17.0)}
	SPListElement e5 = spListElementCreate(5, 6.0); spBPQueueEnqueue(q,e5);//5/5 elements in queue
	ASSERT_TRUE(spBPQueueIsFull(q));
	spBPQueueDestroy(q);
	spListElementDestroy(e1);spListElementDestroy(e2);
	spListElementDestroy(e3);spListElementDestroy(e4);
	spListElementDestroy(e5);
	return true;
}
//Checks case of enqueuing low priority input to a full queue
static bool qFull2() {
	SPBPQueue q = spBPQueueCreate(4);
	SPListElement e1 = spListElementCreate(1, 4.0); spBPQueueEnqueue(q,e1); //Queue: {(1,4.0)}
	SPListElement e2 = spListElementCreate(2, 3.0); spBPQueueEnqueue(q,e2);//Queue: {(2,3.0),(1,4.0)}
	SPListElement e3 = spListElementCreate(3, 17.0); spBPQueueEnqueue(q,e3);//Queue: {(2,3.0),(1,4.0),(3,17.0)}
	SPListElement e4 = spListElementCreate(4, 2.0); spBPQueueEnqueue(q,e4); //Queue: {(4,2.0),(2,3.0),(1,4.0),(3,17.0)}
	SPListElement e5 = spListElementCreate(5, 18.0);
	SPListElement e6;
	ASSERT_TRUE(spBPQueueEnqueue(q,e5) == SP_BPQUEUE_FULL);//Queue doesn't change
	e6 = spBPQueuePeekLast(q);
	ASSERT_TRUE(spListElementGetValue(e3) == spListElementGetValue(e6));
	ASSERT_TRUE(spListElementGetIndex(e3) == spListElementGetIndex(e6));
	spBPQueueDestroy(q);
	spListElementDestroy(e1);spListElementDestroy(e2);
	spListElementDestroy(e3);spListElementDestroy(e4);
	spListElementDestroy(e5);spListElementDestroy(e6);
	return true;
}
//Checks enqueuing by index
static bool qIndexEnqueue() {
	SPBPQueue q = spBPQueueCreate(4);
	SPListElement e1 = spListElementCreate(1, 4.0); spBPQueueEnqueue(q,e1); //Queue: {(1,4.0)}
	SPListElement e2 = spListElementCreate(2, 3.0); spBPQueueEnqueue(q,e2);//Queue: {(2,3.0),(1,4.0)}
	SPListElement e3 = spListElementCreate(3, 17.0); spBPQueueEnqueue(q,e3);//Queue: {(2,3.0),(1,4.0),(3,17.0)}
	SPListElement e4 = spListElementCreate(4, 2.0); spBPQueueEnqueue(q,e4); //Queue: {(4,2.0),(2,3.0),(1,4.0),(3,17.0)}
	SPListElement e5 = spListElementCreate(2, 17.0);//Queue: {(4,2.0),(2,3.0),(1,4.0),(2,17.0)}
	SPListElement e6;
	ASSERT_TRUE(spBPQueueEnqueue(q,e5) == SP_BPQUEUE_SUCCESS);
	e6 = spBPQueuePeekLast(q);
	ASSERT_TRUE(spListElementGetValue(e3) == spListElementGetValue(e6));
	ASSERT_TRUE(spListElementGetIndex(e6) == 2);
	spBPQueueDestroy(q);
	spListElementDestroy(e1);spListElementDestroy(e2);
	spListElementDestroy(e3);spListElementDestroy(e4);
	spListElementDestroy(e5);spListElementDestroy(e6);
	return true;
}
//Checks min value function
static bool qMinValue() {
	SPBPQueue q = spBPQueueCreate(5);
	SPListElement e1 = spListElementCreate(1, 4.0); spBPQueueEnqueue(q,e1); //Queue: {(1,4.0)}
	SPListElement e2 = spListElementCreate(2, 3.0); spBPQueueEnqueue(q,e2);//Queue: {(2,3.0),(1,4.0)}
	SPListElement e3 = spListElementCreate(3, 17.0); spBPQueueEnqueue(q,e3);//Queue: {(2,3.0),(1,4.0),(3,17.0)}
	SPListElement e4 = spListElementCreate(4, 2.0); spBPQueueEnqueue(q,e4); //Queue: {(4,2.0),(2,3.0),(1,4.0),(3,17.0)}
	SPListElement e5 = spListElementCreate(5, 6.0); spBPQueueEnqueue(q,e5);//5/5 elements in queue
	ASSERT_TRUE(spBPQueueMinValue(q) == 2.0);
	spBPQueueDestroy(q);
	spListElementDestroy(e1);spListElementDestroy(e2);
	spListElementDestroy(e3);spListElementDestroy(e4);
	spListElementDestroy(e5);
	return true;
}
//Checks max value function
static bool qMaxValue() {
	SPBPQueue q = spBPQueueCreate(5);
	SPListElement e1 = spListElementCreate(1, 4.0); spBPQueueEnqueue(q,e1); //Queue: {(1,4.0)}
	SPListElement e2 = spListElementCreate(2, 3.0); spBPQueueEnqueue(q,e2);//Queue: {(2,3.0),(1,4.0)}
	SPListElement e3 = spListElementCreate(3, 17.0); spBPQueueEnqueue(q,e3);//Queue: {(2,3.0),(1,4.0),(3,17.0)}
	SPListElement e4 = spListElementCreate(4, 2.0); spBPQueueEnqueue(q,e4); //Queue: {(4,2.0),(2,3.0),(1,4.0),(3,17.0)}
	SPListElement e5 = spListElementCreate(5, 6.0); spBPQueueEnqueue(q,e5);//5/5 elements in queue
	ASSERT_TRUE(spBPQueueMaxValue(q) == 17.0);
	spBPQueueDestroy(q);
	spListElementDestroy(e1);spListElementDestroy(e2);
	spListElementDestroy(e3);spListElementDestroy(e4);
	spListElementDestroy(e5);
	return true;
}
//Checks copy function
static bool qCopy() {
	SPBPQueue q = spBPQueueCreate(5);
	SPBPQueue qCopy;
	SPListElement e1 = spListElementCreate(1, 4.0); spBPQueueEnqueue(q,e1); //Queue: {(1,4.0)}
	SPListElement e2 = spListElementCreate(2, 3.0); spBPQueueEnqueue(q,e2);//Queue: {(2,3.0),(1,4.0)}
	SPListElement e3 = spListElementCreate(3, 17.0); spBPQueueEnqueue(q,e3);//Queue: {(2,3.0),(1,4.0),(3,17.0)}
	SPListElement e4 = spListElementCreate(4, 2.0); spBPQueueEnqueue(q,e4); //Queue: {(4,2.0),(2,3.0),(1,4.0),(3,17.0)}
	SPListElement e5 = spListElementCreate(5, 6.0); spBPQueueEnqueue(q,e5);//5/5 elements in queue
	qCopy = spBPQueueCopy(q);
	SPListElement peek1 = spBPQueuePeek(q);
	SPListElement peek2 = spBPQueuePeek(qCopy);
	ASSERT_TRUE(spListElementGetValue(peek1) == spListElementGetValue(peek2));
	ASSERT_TRUE(spListElementGetIndex(peek1) == spListElementGetIndex(peek2));
	spListElementDestroy(peek1);
	spListElementDestroy(peek2);
	for (int i=0; i<4; i++) {
		spBPQueueDequeue(q);
		spBPQueueDequeue(qCopy);
		peek1 = spBPQueuePeek(q);
		peek2 = spBPQueuePeek(qCopy);
		ASSERT_TRUE(spListElementGetValue(peek1) == spListElementGetValue(peek2));
		ASSERT_TRUE(spListElementGetIndex(peek1) == spListElementGetIndex(peek2));
		spListElementDestroy(peek1);spListElementDestroy(peek2);
	}
	spBPQueueDestroy(q);
	spBPQueueDestroy(qCopy);
	spListElementDestroy(e1);spListElementDestroy(e2);
	spListElementDestroy(e3);spListElementDestroy(e4);
	spListElementDestroy(e5);
	return true;
}
//Checks peek function
static bool qPeek() {
	SPBPQueue q = spBPQueueCreate(5);
	SPListElement e1 = spListElementCreate(1, 4.0); spBPQueueEnqueue(q,e1); //Queue: {(1,4.0)}
	SPListElement e2 = spListElementCreate(2, 3.0); spBPQueueEnqueue(q,e2);//Queue: {(2,3.0),(1,4.0)}
	SPListElement peek1 = spBPQueuePeek(q);
	ASSERT_TRUE(spListElementGetValue(peek1) == 3.0);
	ASSERT_TRUE(spListElementGetIndex(peek1) == 2);
	spBPQueueDestroy(q);
	spListElementDestroy(e1);spListElementDestroy(e2);
	spListElementDestroy(peek1);
	return true;
}
//Checks peek last function
static bool qPeekLast() {
	SPBPQueue q = spBPQueueCreate(5);
	SPListElement e1 = spListElementCreate(1, 4.0); spBPQueueEnqueue(q,e1); //Queue: {(1,4.0)}
	SPListElement e2 = spListElementCreate(2, 3.0); spBPQueueEnqueue(q,e2);//Queue: {(2,3.0),(1,4.0)}
	SPListElement peek1 = spBPQueuePeekLast(q);
	ASSERT_TRUE(spListElementGetValue(peek1) == 4.0);
	ASSERT_TRUE(spListElementGetIndex(peek1) == 1);
	spBPQueueDestroy(q);
	spListElementDestroy(e1);spListElementDestroy(e2);
	spListElementDestroy(peek1);
	return true;
}

int main() {
	RUN_TEST(qCreate);
	RUN_TEST(qNull);
	RUN_TEST(qEmpty);
	RUN_TEST(qMaxSize);
	RUN_TEST(qSize);
	RUN_TEST(qFull);
	RUN_TEST(qFull2);
	RUN_TEST(qIndexEnqueue);
	RUN_TEST(qMinValue);
	RUN_TEST(qMaxValue);
	RUN_TEST(qCopy);
	RUN_TEST(qPeek);
	RUN_TEST(qPeekLast);
	return 0;
}
