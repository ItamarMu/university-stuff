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

void signal_handler (int signum) {
	int fd, fsize, errchk, numofA = 0, i = 0;
	char *arr;
	struct timeval t1, t2;
	double elapsed_milisec;
	fd = open(DEST,O_RDWR,0600);
	if (fd == -1) {
		printf("Error creating file: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	fsize = lseek(fd,0,SEEK_END);
	if (fsize == -1) {
		printf("Error calling lseek() to get the size of the file: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	fsize++; //we need the size, not the offset
	errchk = gettimeofday(&t1, NULL);
	if (errchk == -1) {
		printf("Error getting time: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	arr = (char*) mmap(NULL,fsize,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
	if (arr == MAP_FAILED) {
		printf("Error mapping the file: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	while (arr[i] != '\0' && i < fsize) {
		if (arr[i] == 'a')
			numofA++;
		i++;
    }
    errchk = gettimeofday(&t2, NULL);
	if (errchk == -1) {
		printf("Error getting time: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	errchk = munmap(arr,fsize);
    if (errchk == -1) {
    	printf("Error un-mapping the file: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
    }
	elapsed_milisec = (t2.tv_sec - t1.tv_sec) * 1000.0;
	elapsed_milisec += (t2.tv_usec - t1.tv_usec) / 1000.0;
	printf("%d were read in %f miliseconds through MMAP\n", numofA+1, elapsed_milisec);
	errchk = unlink(DEST);
	if (errchk == -1) {
    	printf("Error removing the file: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
    }
    close(fd);
    exit(EXIT_SUCCESS);
}

int main(void) {
	struct sigaction new_action, new_action1;
	new_action.sa_handler = signal_handler;
	new_action.sa_flags = 0;
	new_action1.sa_handler = SIG_IGN;
	new_action1.sa_flags = 0;
	if (sigaction (SIGUSR1, &new_action, NULL) != 0) {
		printf("Signal handle registration failed. %s\n",strerror(errno));
		return -1;
	}
	if (sigaction (SIGTERM, &new_action1, NULL) != 0) {
		printf("Signal handle registration failed. %s\n",strerror(errno));
		return -1;
	}
	while(1) {
	sleep(2);
	}
	new_action.sa_handler = SIG_DFL;
	new_action1.sa_handler = SIG_DFL;
	return EXIT_SUCCESS;
}