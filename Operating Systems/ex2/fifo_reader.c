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

#define BUFSZ 4096
#define DEST "tmp/osfifo"

int main(void) {
	int fd, errchk, Acount = 0;
	long int NUM;
	char* fif = DEST;
	char Abuff[BUFSZ];
	struct timeval t1, t2;
	double elapsed_milisec;
	struct sigaction new_action;
	new_action.sa_handler = SIG_IGN;
	new_action.sa_flags = 0;
	if (sigaction (SIGINT, &new_action, NULL) != 0) {
		printf("Signal handle registration failed. %s\n",strerror(errno));
		return -1;
	}
	fd = open(fif,O_RDONLY,0600);
	if (fd == -1) {
		printf("Error openning the file: %s\n", strerror(errno));
		return -1;
	}
	errchk = gettimeofday(&t1, NULL);
	if (errchk == -1) {
		printf("Error getting time: %s\n", strerror(errno));
		return -1;
	}
	while (read(fd,Abuff,BUFSZ)) {
		for (int i = 0; i < BUFSZ; i++) {
			if (Abuff[i] == 'a')
				Acount++;
		}
	}
	errchk = gettimeofday(&t2, NULL);
	if (errchk == -1) {
		printf("Error getting time: %s\n", strerror(errno));
		return -1;
	}
	elapsed_milisec = (t2.tv_sec - t1.tv_sec) * 1000.0;
	elapsed_milisec += (t2.tv_usec - t1.tv_usec) / 1000.0;
	printf("%d were read in %f miliseconds through FIFO\n",Acount,elapsed_milisec);
	close(fd);
	new_action.sa_handler = SIG_DFL;
	return 0;
}