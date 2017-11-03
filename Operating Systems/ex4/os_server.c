#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <assert.h>
#include <linux/random.h> 
#include <fcntl.h>
#include <signal.h>

#define BUF_SIZE 4096
int go = 1;

void serverStop() {
	go = 0;
}

int main(int argc, char *argv[]) {
	int fdkey, nsent, n = 0, listenfd = 0, connfd = 0, prcid, errnum, keyBuffIndex, readCount = 0, writeCount = 0, alreadyReadForKey = 0, keyBuffNum = 0;
	struct sockaddr_in serv_addr, peer_addr;  
	struct stat keyStat;
	struct sigaction command;
	command.sa_handler = serverStop;
	command.sa_flags = 0;
	char sendBuff[BUF_SIZE];
	char keyBuff[BUF_SIZE];
	errnum = sigaction(SIGINT,&command,NULL);
	if (errnum != 0) {
		printf("Error while registerring SIGINT: %s\n", strerror(errno));
		return -1;
	}
	//check arguments
	if (argc == 4) {
		int keylen = strtol(argv[3],NULL,10);
		if (errno == ERANGE || errno == EINVAL) {
			printf("Error reading input keylen: %s\n", strerror(errno));
			return -1;
		}
		int RNDdata = open("/dev/urandom", O_RDONLY);
		if (RNDdata < 0) {
			printf("Error openning /dev/urandom: %s\n", strerror(errno));
			return -1;
		}
		fdkey = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (fdkey < 0) {
			printf("Error creating key file: %s\n", strerror(errno));
			return -1;
		}

		while (alreadyReadForKey < keylen) {
			if (keylen - alreadyReadForKey < BUF_SIZE)
				keyBuffNum = keylen - alreadyReadForKey;
			else
				keyBuffNum = BUF_SIZE;
			n = read(RNDdata, keyBuff, keyBuffNum);
			if (n < 0) {
				printf("Error reading random data file: %s\n", strerror(errno));
				return -1;
			}
			keyBuffIndex = 0;
			while (keyBuffIndex < n) {
				writeCount = write(fdkey, keyBuff + keyBuffIndex, n - keyBuffIndex);
				if (writeCount < 0) {
					printf("Error writing key file: %s\n", strerror(errno));
					return -1;
				}
				keyBuffIndex += writeCount;
			}
			alreadyReadForKey += n;
		}

		errnum = stat(argv[2],&keyStat);
		if (errnum < 0) {
			printf("Error while using stat() function: %s\n", strerror(errno));
			return -1;
		}
		close(RNDdata);
		close(fdkey);
	}
	else {
		if (argc != 3) {
			printf("Invalid number of arguments\n");
			return -1;
		}
	else { //no keylen provided
		errnum = stat(argv[2],&keyStat);
		if (errnum < 0) {
			printf("Error while using stat() function: %s\n", strerror(errno));
			return -1;
		}
		if (S_ISDIR(keyStat.st_mode) ||  keyStat.st_size == 0) {
			printf("Error in key file provided\n");
			return -1;
		}
	}
}

short port = strtol(argv[1],NULL,10);
if (errno == ERANGE || errno == EINVAL) {
	printf("Error reading input port: %s\n", strerror(errno));
	return -1;
}

listenfd = socket(AF_INET, SOCK_STREAM, 0);
if (listenfd < 0) {
	printf("Error creating socket\n");
	return -1;
}

serv_addr.sin_family = AF_INET;
serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
serv_addr.sin_port = htons(port); 
if(bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))){
	printf("Error using bind(): %s\n", strerror(errno));
	return -1; 
}

if(listen(listenfd, 10)){
	printf("Error using listen(): %s\n", strerror(errno));
	return -1; 
}
while(go)
{
	socklen_t addrsize = sizeof(struct sockaddr_in );
	connfd = accept(listenfd, (struct sockaddr*)&peer_addr, &addrsize);
	if(connfd < 0) {
		if (go == 1) {
			printf("\n Error : Accept Failed. %s \n", strerror(errno));
			return -1; 
		}
		else {
			close(connfd);
			break;
		}
	}

	prcid = fork();
	if (prcid < 0) {
		printf("Error using fork(): %s\n", strerror(errno));
		return -1;
	}
	if (prcid == 0) {
		close(listenfd);
		fdkey = open(argv[2], O_RDONLY, 0777);
		if (fdkey < 0) {
			printf("Error openning key file: %s\n", strerror(errno));
			return -1;
		}
		// handle a chunk of data
		while((n = read(connfd,&sendBuff, BUF_SIZE)) > 0 ) {
			//build keychunk
			keyBuffIndex = 0;
			while (keyBuffIndex < n) {
				readCount = read(fdkey, keyBuff + keyBuffIndex, n - keyBuffIndex);
				if (readCount < 0) {
					printf("Error reading key file: %s\n", strerror(errno));
					return -1;
				}
				if (readCount == 0) {
					if (lseek(fdkey, 0, SEEK_SET) < 0) {
						printf("Error using lseek(): %s\n", strerror(errno));
						return -1;
					}
				}
				if ((BUF_SIZE - keyBuffIndex) / keyStat.st_size > 2 && readCount == keyStat.st_size) {
					for (int i = 0; i < (BUF_SIZE - keyBuffIndex) / keyStat.st_size - 1; i++) {
						for (int j = 0; j < keyStat.st_size; j++) {
							keyBuff[keyBuffIndex + readCount + i*readCount + j] = keyBuff[keyBuffIndex + j];
						}
					}
					keyBuffIndex += ((BUF_SIZE - keyBuffIndex) / keyStat.st_size -1)*keyStat.st_size;
				}
				keyBuffIndex += readCount;
			}

			for (int i = 0; i < n; i++)
				sendBuff[i] = sendBuff[i] ^ keyBuff[i];

			keyBuffIndex = 0;
			while (keyBuffIndex < n) {
				nsent = write(connfd, sendBuff + keyBuffIndex, n - keyBuffIndex);
				if (nsent < 0) {
					printf("Error while sending data: %s\n", strerror(errno));
					return -1;
				}
				keyBuffIndex += nsent;
			}

		}//child process loop
		close(fdkey);
		return 0;
	}//child process if

	close(connfd);
}//server loop
close(listenfd);
return 0;
}