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

void signal_handler (int signum) {
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
	int fd, errchk, writtenA = 0;
	long int NUM;
	char* fif = DEST;
	char Abuff[BUFSZ] = { [0 ... 4095] = 'a'}; //credit: http://stackoverflow.com/questions/1065774/initialization-of-a-normal-array-with-one-default-value
	struct timeval t1, t2;
	double elapsed_milisec;
	struct sigaction new_action, new_action1;
	new_action.sa_handler = signal_handler;
	new_action.sa_flags = 0;
	new_action1.sa_handler = SIG_IGN;
	new_action1.sa_flags = 0;
	if (sigaction (SIGPIPE, &new_action, NULL) != 0) {
		printf("Signal handle registration failed. %s\n",strerror(errno));
		return -1;
	}
	if (sigaction (SIGINT, &new_action1, NULL) != 0) {
		printf("Signal handle registration failed. %s\n",strerror(errno));
		return -1;
	}
	NUM = strtol(argv[1],NULL,10);
	if (errno == ERANGE) {
		printf("Error reading input number: %s\n", strerror(errno));
		return -1;
	}
	errchk = mkfifo(fif,0600);
	if (errchk == -1) {
		printf("Error calling mkfifo(): %s\n", strerror(errno));
		return -1;
	}
	fd = open(fif,O_WRONLY,0600);
	if (fd == -1) {
		printf("Error openning the file: %s\n", strerror(errno));
		return -1;
	}
	errchk = gettimeofday(&t1, NULL);
	if (errchk == -1) {
		printf("Error getting time: %s\n", strerror(errno));
		return -1;
	}
	while (writtenA <= NUM - BUFSZ) {
		write(fd, Abuff, BUFSZ);
		writtenA += BUFSZ;
	}
	while (writtenA < NUM) {
		write(fd, "a", 1);
		writtenA++;
	}
	errchk = gettimeofday(&t2, NULL);
	if (errchk == -1) {
		printf("Error getting time: %s\n", strerror(errno));
		return -1;
	}
	elapsed_milisec = (t2.tv_sec - t1.tv_sec) * 1000.0;
	elapsed_milisec += (t2.tv_usec - t1.tv_usec) / 1000.0;
	printf("%d were written in %f miliseconds through FIFO\n",writtenA,elapsed_milisec);
	new_action.sa_handler = SIG_DFL;
	new_action1.sa_handler = SIG_DFL;
	close(fd);
	unlink(fif);
	return 0;
}