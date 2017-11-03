#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <errno.h>
#include <fcntl.h>

#define BUF_SIZE 4096

int main(int argc, char *argv[]) {

	int fdsrc, fddst, errnum, sockfd, n, index = 0, num;
	char readbuf[BUF_SIZE], writebuf[BUF_SIZE];
	short port = strtol(argv[2],NULL,10);
	if (errno == ERANGE || errno == EINVAL) {
		printf("Error reading input port: %s\n", strerror(errno));
		return -1;
	}
	fdsrc = open(argv[3], O_RDONLY);
	if (fdsrc < 0) {
		printf("Error while openning IN file: %s\n",strerror(errno));
		return -1;
	}
	fddst = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (fddst < 0) {
			printf("Error while openning OUT file %s\n", strerror(errno));
			return -1;
		}

	struct sockaddr_in serv_addr = {0}; 

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Error creating socket\n");
        return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]); 
    if (serv_addr.sin_addr.s_addr == (in_addr_t)(-1)) {
    	printf("Error using inet_addr()\n");
    	return -1;
    }
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("Error using connect(): %s\n", strerror(errno));
       return -1;
    }

    while ((n = read(fdsrc, readbuf, BUF_SIZE)) > 0) {
    	index = 0;
    	while (index < n) {
    		num = write(sockfd, readbuf + index, n - index);
    		if (num < 0) {
    			printf("Error writing to socket: %s\n", strerror(errno));
    			return -1;
    		}
    		index += num;
    	}
    	index = 0; 
    	while (index < n) {
    		num = read(sockfd, writebuf + index, n - index);
    		if (num < 0) {
    			printf("Error reading from socket: %s\n", strerror(errno));
    			return -1;
    		}
    		index += num;
    	}
    	index = 0;
    	while (index < n) {
    		num = write(fddst, writebuf + index, n - index);
    		if (num < 0) {
    			printf("Error writing to file: %s\n", strerror(errno));
    			return -1;
    		}
    		index += num;
    	}
    }

    if(n < 0) 
    {
    	printf("Error reading from source file: %s\n", strerror(errno));
    	return -1;
    }

    close(sockfd);
    close(fddst);
    close(fdsrc);
	return 0;
}