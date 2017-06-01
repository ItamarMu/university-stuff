#ifndef SPBPRIORITYQUEUE_H_
#define SPBPRIORITYQUEUE_H_
#include "SPListElement.h"
#include <stdbool.h>
/**
Bounded Priority Queue Container Summary
 *
 * Implements a Bounded Priority Queue container type.
 * The elements of the list are managed by the SPList type, please refer to
 * SPList.h for usage.
 *
 * The following functions are available:
 *
 *   spBPQueueCreate            - Creates an empty queue with a given maximum capacity
 *   spBPQueueCopy              - Creates a copy of a given queue
 *   spBPQueueDestroy           - Frees all memory allocation associated with the queue
 *   spBPQueueClear             - Removes all the elements in the queue
 *   spBPQueueSize              - Returns the number of elements in the queue
 *   spBPQueueGetMaxSize        - Returns the maximum capacity of the queue
 *   spBPQueueEnqueue           - Inserts a NEW COPY (must be allocated) element to the queue
 *   spBPQueueDequeue           - Removes the element with the lowest value
 *   spBPQueuePeek          	- Returns a NEW COPY of the element with the lowest value
 *   spBPQueuePeekLast          - Returns a NEW COPY of the element with the highest value
 *   spBPQueueMinValue  		- Returns the minimum value in the queue
 *   spBPQueueMaxValue          - Returns the maximum value in the queue
 *   spBPQueueIsEmpty   		- Returns true if the queue is empty
 *   spBPQueueIsFull            - Returns true if the queue is full
 */

/** type used to define Bounded priority queue **/
typedef struct sp_bp_queue_t* SPBPQueue;

/** type for error reporting **/
typedef enum sp_bp_queue_msg_t {
	SP_BPQUEUE_OUT_OF_MEMORY,
	SP_BPQUEUE_FULL,
	SP_BPQUEUE_EMPTY,
	SP_BPQUEUE_INVALID_ARGUMENT,
	SP_BPQUEUE_SUCCESS
} SP_BPQUEUE_MSG;

/**
 * Allocates a new Bounded Priority Queue.
 *
 * This function creates a new empty BPQ.
 * @return
 * 	NULL - If allocations failed.
 * 	A new List in case of success.
 */
SPBPQueue spBPQueueCreate(int maxSize);

/**
 * Creates a copy of a given Bounded Priority Queue.
 *
 * The new copy will contain all the elements from the source gueue in the same
 * order and will have the same maximum size.
 *
 * @param source The source queue to be copied.
 * @return
 * NULL if a NULL was sent or a memory allocation failed.
 * A BPQ containing the same elements with same order as source otherwise.
 */
SPBPQueue spBPQueueCopy(SPBPQueue source);

/**
 * Deallocates an existing Bounded Priority Queue.
 * Clears all elements by using the
 * stored free function.
 *
 * @param source Target queue to be deallocated. If queue is NULL nothing will be
 * done
 */
void spBPQueueDestroy(SPBPQueue source);

/**
 * Removes all elements from target Bounded Priority Queue.
 * The elements are deallocated using the stored freeing function.
 * @param source Target queue to remove all element from.
 */
void spBPQueueClear(SPBPQueue source);

/**
 * Returns the number of elements in a Bounded Priority Queue.
 * @param source The target queue which size is requested.
 * @return
 * -1 if a NULL pointer was sent.
 * Otherwise the number of elements in the list.
 */
int spBPQueueSize(SPBPQueue source);

/**
 * Returns the maximum size of a given Bounded Priority Queue.
 * @param source The target queue which size is requested.
 * @return
 * -1 if a NULL pointer was sent.
 * Otherwise the maximum number of elements in the queue.
 */
int spBPQueueGetMaxSize(SPBPQueue source);

/**
 * Adds a new element to the Bounded Priority Queue, the new element will be placed
 * before the first element with a higher priority value. If no such element exists,
 * the new element will be placed last in the queue as long as the queue size is smaller than
 * its max size. Otherwise, the new element will not be added at all.
 * @param source The queue for which to add an element.
 * @param element The element to insert. A copy of the element will be
 * inserted.
 * @return
 * SP_BPQUEUE_INVALID_ARGUMENT if a NULL was sent as queue or element.
 * SP_BPQUEUE_FULL if the queue is full and the element wasn't inserted.
 * SP_BPQUEUE_OUT_OF_MEMORY if an allocation failed.
 * SP_BPQUEUE_SUCCESS the element has been inserted successfully.
 */
SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element);

/**
 * Removes the element with the lowest value in the Bounded Priority Queue.
 * @param source the queue for which the current element will be removed
 * @return
 * SP_BPQUEUE_INVALID_ARGUMENT if source is NULL
 * SP_BPQUEUE_EMPTY if source is empty
 * SP_BPQUEUE_SUCCESS if the first element in the queue was successfully removed.
 */
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source);

/**
 * Returns a new copy of the element with the lowest value.
 * @param source the queue for which the element with the lowest value will be copied.
 * @return
 * NULL if source is NULL or an allocation error occurred.
 * Otherwise, the requested copy.
 */
SPListElement spBPQueuePeek(SPBPQueue source);

/**
 * Returns a new copy of the element with the highest value.
 * @param source The queue for which the element with the lowest value will be copied.
 * @return
 * NULL if source is NULL or an allocation error occurred.
 * Otherwise, the requested copy.
 */
SPListElement spBPQueuePeekLast(SPBPQueue source);

/**
 * Returns the minimum value in the queue.
 * @param source The queue for which the lowest value will be copied.
 * @return
 * -1 if source is NULL or empty.
 * Otherwise, the value of the element with the lowest value.
 */
double spBPQueueMinValue(SPBPQueue source);

/**
 * Returns the maximum value in the queue.
 * @param source The queue for which the highest value will be copied.
 * @return
 * -1 if source is NULL or empty.
 * Otherwise, the value of the element with the highest value.
 */
double spBPQueueMaxValue(SPBPQueue source);

/**
 * Returns True if the queue is empty.
 * @assert source != NULL
 * @param source The queue to be examined.
 * @return
 * True if source contains at least one element.
 * Otherwise, False.
 */
bool spBPQueueIsEmpty(SPBPQueue source);

/**
 * Returns True if the element count in queue equals the queue's max size.
 * @assert source != NULL
 * @param source The queue to be examined.
 * @return
 * True if source->maxSize == source's element count.
 * Otherwise, False.
 */
bool spBPQueueIsFull(SPBPQueue source);

#endif
