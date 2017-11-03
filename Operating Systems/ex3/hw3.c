#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <pthread.h>

struct Node {
	int value;
	struct Node* next;
	struct Node* prev;
};

typedef struct intlist {
	int size;
	struct Node* head;
	struct Node* tail;
	pthread_mutexattr_t attr;
	pthread_mutex_t lock;
	pthread_cond_t not_empty;
}intlist;

void intlist_init(intlist* list);
void intlist_destroy(intlist* list);
pthread_mutex_t* intlist_get_mutex(intlist* list);
void intlist_push_head(intlist* list, int value);
int intlist_pop_tail(intlist* list);
void intlist_remove_last_k(intlist* list, int k);
int intlist_size(intlist* list);

// int errnum;



struct Node* create_node (int val) {
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
	if (newNode == NULL) {
		printf("Error in create_node, malloc failed\n");
		exit(1);
	}
	newNode->value = val;
	newNode->prev = NULL;
	newNode->next = NULL;
	return newNode;
}

void destroy_node (struct Node* node) {
	if (node == NULL) {
		printf("Error while destroying node, node is NULL\n");
		exit(1);
	}
	else
		free(node);
}

void intlist_init(intlist* list) {
	int errnum;
	list->size = 0;
	list->head = create_node(0);
	list->tail = create_node(0);//head and tail use as a 'cover', not as real nodes
	list->head->next = list->tail;
	list->tail->prev = list->head;
	errnum = pthread_cond_init(&list->not_empty, NULL);
	if (errnum != 0) {
		printf("Error while initializing thread condition: %s\n", strerror(errnum));
		exit(1);
	}
	errnum = pthread_mutexattr_init(&list->attr);
	if (errnum != 0) {
		printf("Error while initializing attribute: %s\n", strerror(errnum));
		exit(1);
	}
	errnum = pthread_mutexattr_settype(&list->attr,PTHREAD_MUTEX_RECURSIVE);
	if (errnum != 0) {
		printf("Error while setting attribute's type: %s\n", strerror(errnum));
		exit(1);
	}
	errnum = pthread_mutex_init(intlist_get_mutex(list), &list->attr);
	if (errnum != 0) {
		printf("Error while initializing mutex: %s\n", strerror(errnum));
		exit(1);
	}
}

void intlist_destroy(intlist* list) {
	int errnum;
	while (list->head->next != list->tail) {
		list->head->next = list->head->next->next;
		destroy_node(list->head->next->prev);
		list->head->next->prev = list->head;
	}
	destroy_node(list->head);
	destroy_node(list->tail);
	errnum = pthread_mutex_destroy(intlist_get_mutex(list));
	if (errnum != 0) {
		printf("Error while destroying mutex: %s\n", strerror(errnum));
		exit(1);
	}
	errnum = pthread_cond_destroy(&list->not_empty);
	if (errnum != 0) {
		printf("Error while destroying thread condition: %s\n", strerror(errnum));
		exit(1);
	}
	errnum = pthread_mutexattr_destroy(&list->attr);
	if (errnum != 0) {
		printf("Error while destroying attribute: %s\n", strerror(errnum));
		exit(1);
	}
}

pthread_mutex_t* intlist_get_mutex(intlist* list) {
	return &list->lock;
}

void intlist_push_head(intlist* list, int value) {
	int errnum;
	struct Node* node = create_node(value);
	errnum = pthread_mutex_lock(intlist_get_mutex(list));
	if (errnum != 0) {
		printf("Error while locking: %s\n", strerror(errnum));
		exit(1);
	}
	node->next = list->head->next;
	node->prev = list->head;
	list->head->next->prev = node;
	list->head->next = node;
	list->size++;
	errnum = pthread_cond_signal(&list->not_empty);
	if (errnum != 0) {
		printf("Error while activating conditional signal: %s\n", strerror(errnum));
		exit(1);
	}
	errnum = pthread_mutex_unlock(intlist_get_mutex(list));
	if (errnum != 0) {
		printf("Error while unlocking: %s\n", strerror(errnum));
		exit(1);
	}
}

int intlist_pop_tail(intlist* list) {
	int errnum, val = 0;
	struct Node* tmp;
	errnum = pthread_mutex_lock(intlist_get_mutex(list));
	if (errnum != 0) {
		printf("Error while locking: %s\n", strerror(errnum));
		exit(1);
	}
	while (list->size == 0) {
		errnum = pthread_cond_wait(&list->not_empty,intlist_get_mutex(list));
		if (errnum != 0) {
			printf("Error while waiting for condition: %s\n", strerror(errnum));
			exit(1);
		}
	}
	tmp = list->tail->prev;
	val = tmp->value;
	tmp->prev->next = list->tail;
	list->tail->prev = tmp->prev;
	destroy_node(tmp);
	list->size--;
	errnum = pthread_mutex_unlock(intlist_get_mutex(list));
	if (errnum != 0) {
		printf("Error while unlocking: %s\n", strerror(errnum));
		exit(1);
	}
	return val;
}

void intlist_remove_last_k(intlist* list, int k) {
	int errnum, i = 0;
	errnum = pthread_mutex_lock(intlist_get_mutex(list));
	if (errnum != 0) {
		printf("Error while locking: %s\n", strerror(errnum));
		exit(1);
	}
	while (list->size > 0 && i < k) {
		intlist_pop_tail(list);
		i++;
	}
	errnum = pthread_mutex_unlock(intlist_get_mutex(list));
	if (errnum != 0) {
		printf("Error while unlocking: %s\n", strerror(errnum));
		exit(1);
	}
}

int intlist_size(intlist* list) {
	return list->size;
}

intlist ls;
pthread_cond_t full;
int goGC = 1, goW = 1, goR = 1;
long int MAX;

void *GCF() {
	int errnum, x = 0;
	while(goGC) {
		errnum = pthread_mutex_lock(intlist_get_mutex(&ls));
		if (errnum != 0) {
			printf("Error in GCF - mutex_lock: %s\n",strerror(errnum));
			exit(1);
		}
		while (intlist_size(&ls) <= MAX && goGC) {
			errnum = pthread_cond_wait(&full,intlist_get_mutex(&ls));
			if (errnum != 0) {
				printf("Error while waiting for condition in GCF: %s\n", strerror(errnum));
				exit(1);
			}
		}
		if (goGC) {
			x = (intlist_size(&ls)+1)/2;
			intlist_remove_last_k(&ls,x);
			printf("GC - %d items removed from the list\n",x);
		}
		errnum = pthread_mutex_unlock(intlist_get_mutex(&ls));
		if (errnum != 0) {
			printf("Error in GCF - mutex_unlock: %s\n",strerror(errnum));
			exit(1);
		}
	}
	pthread_exit(NULL);
}

void *writerF() {
	int errnum;
	while(goW) {
		intlist_push_head(&ls,rand());
		if (intlist_size(&ls) > MAX) {
			errnum = pthread_cond_signal(&full);
			if (errnum != 0) {
				printf("Error in writerF - cond_signal: %s\n",strerror(errnum));
				exit(1);
			}
		}
	}
	pthread_exit(NULL);
}


void *readerF() {
	int errnum;
	while(goR) {
		if (intlist_size(&ls) > MAX) {
			errnum = pthread_cond_signal(&full);
			if (errnum != 0) {
				printf("Error in writerF - cond_signal: %s\n",strerror(errnum));
				exit(1);
			}
		}
		intlist_pop_tail(&ls);
	}
	pthread_exit(NULL);
}

int main(int argc, char** argv) {
	intlist_init(&ls);
	long int WNUM, RNUM, TIME;
	int errnum;
	WNUM = strtol(argv[1],NULL,10);
	if (errno == ERANGE || errno == EINVAL) {
		printf("Error reading input 1: %s\n", strerror(errno));
		return -1;
	}
	RNUM = strtol(argv[2],NULL,10);
	if (errno == ERANGE || errno == EINVAL) {
		printf("Error reading input 2: %s\n", strerror(errno));
		return -1;
	}
	MAX = strtol(argv[3],NULL,10);
	if (errno == ERANGE || errno == EINVAL) {
		printf("Error reading input 3: %s\n", strerror(errno));
		return -1;
	}
	TIME = strtol(argv[4],NULL,10);
	if (errno == ERANGE || errno == EINVAL) {
		printf("Error reading input 4: %s\n", strerror(errno));
		return -1;
	}
	pthread_t GC;
	pthread_t writers[WNUM];
	pthread_t readers[RNUM];
	errnum = pthread_cond_init (&full, NULL);
	if (errnum != 0) {
		printf("Error while initializing condition %s\n",strerror(errnum));
		return -1;
	}
	errnum = pthread_create(&GC, NULL, GCF, NULL);
	if (errnum != 0) {
		printf("Error while creating garbage collector %s\n",strerror(errnum));
		return -1;
	}
	for (int i = 0; i < WNUM; i++) {
		errnum = pthread_create(&writers[i], NULL, writerF, NULL);
		if (errnum != 0) {
			printf("Error while creating writers %s\n",strerror(errnum));
			return -1;
		}
	}
	for (int i = 0; i < RNUM; i++) {
		errnum = pthread_create(&readers[i], NULL, readerF, NULL);
		if (errnum != 0) {
			printf("Error while creating readers %s\n",strerror(errnum));
			return -1;
		}
	}
	sleep(TIME);
	goGC = 0;
	errnum = pthread_cond_signal(&full);
	if (errnum != 0) {
		printf("Error while using cond_signal %s\n",strerror(errnum));
		return -1;
	}
	errnum = pthread_join(GC, NULL);
	if (errnum != 0) {
		printf("Error while joining garbage collector %s\n",strerror(errnum));
		return -1;
	}
	goR = 0;
	for (int i = 0; i < RNUM; i++) {
		errnum = pthread_join(readers[i], NULL);
		if (errnum != 0) {
			printf("Error while joining readers %s\n",strerror(errnum));
			return -1;
		}
	}
	goW = 0;
	for (int i = 0; i < WNUM; i++) {
		errnum = pthread_join(writers[i], NULL);
		if (errnum != 0) {
			printf("Error while joining writers %s\n",strerror(errnum));
			return -1;
		}
	}
	printf("The size of the list is %d\n And it contains: ",intlist_size(&ls));
	struct Node* nd = ls.head;
	for (int i = 0; i<intlist_size(&ls); i++) {
		nd = nd->next;
		printf("%d ",nd->value);
	}
	printf("\n");
	//DESTRUCTION
	errnum = pthread_cond_destroy(&full);
	if (errnum != 0) {
		printf("Error using cond_destroy: %s\n", strerror(errnum));
		return -1;
	}
	intlist_destroy(&ls);
	pthread_exit(NULL);
}