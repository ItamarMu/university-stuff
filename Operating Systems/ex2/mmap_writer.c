#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define FILESIZE 4000
#define DEST "tmp/mmapped.bin"

int main(int argc, char *argv[]) {
	int fd, errchk;
	long int NUM, RPID;
	char *arr;
	struct timeval t1, t2;
	double elapsed_milisec;
	struct sigaction new_action;
	new_action.sa_handler = SIG_IGN;
	new_action.sa_flags = 0;
	if (sigaction (SIGTERM, &new_action, NULL) != 0) {
		printf("Signal handle registration failed. %s\n",strerror(errno));
		return -1;
	}
	NUM = strtol(argv[1],NULL,10);
	if (errno == ERANGE) {
		printf("Error reading first input number: %s\n", strerror(errno));
		return -1;
	}
	RPID = strtol(argv[2],NULL,10);
	if (errno == ERANGE) {
		printf("Error reading second input number: %s\n", strerror(errno));
		return -1;
	}
	fd = open(DEST,O_RDWR | O_CREAT | O_TRUNC,0600);
	if (fd == -1) {
		printf("Error creating file: %s\n", strerror(errno));
		return -1;
	}
	errchk = lseek(fd,NUM-1,SEEK_SET);
	if (errchk == -1) {
		printf("Error calling lseek() to 'stretch' the file: %s\n", strerror(errno));
		return -1;
	}
	errchk = write(fd, "", 1);
	if (errchk != 1) {
		printf("Error writing last byte of the file: %s\n", strerror(errno));
		return -1;
	}
	arr = (char*) mmap(NULL,NUM,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
	if (arr == MAP_FAILED) {
		printf("Error mapping the file: %s\n", strerror(errno));
		return -1;
	}
	errchk = gettimeofday(&t1, NULL);
	if (errchk == -1) {
		printf("Error getting time: %s\n", strerror(errno));
		return -1;
	}
	for (int i = 0; i < NUM-1; i++) {
    arr[i] = 'a';
    }
    arr[NUM-1] = '\0';
    errchk = munmap(arr,NUM);
    if (errchk == -1) {
    	printf("Error un-mapping the file: %s\n", strerror(errno));
		return -1;
    }
    errchk = kill(RPID,SIGUSR1);
    if (errchk == -1) {
		printf("Error sending signal: %s\n", strerror(errno));
		return -1;
	}
    errchk = gettimeofday(&t2, NULL);
	if (errchk == -1) {
		printf("Error getting time: %s\n", strerror(errno));
		return -1;
	}
    elapsed_milisec = (t2.tv_sec - t1.tv_sec) * 1000.0;
	elapsed_milisec += (t2.tv_usec - t1.tv_usec) / 1000.0;
	printf("%ld were written in %f miliseconds through MMAP\n", NUM, elapsed_milisec);
    close(fd);
    new_action.sa_handler = SIG_DFL;
	return 0;
}