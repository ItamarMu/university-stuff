// ChannelModule.cpp : Defines the entry point for the console application.
//

#include <stdafx.h>
#include <cmath>
#include <stdlib.h>
#include <winsock2.h>
#include <stdio.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#define MSG_SIZE 63

//Turns byte to 8-bit representaion (in int array)
void F1Bto8bits(char byte, int* bits) {
	int msk = 1;
	for (int i = 0; i < 8; i++) {
		if (byte & msk)
			bits[7 - i] = 1;
		else
			bits[7 - i] = 0;
		msk = msk << 1;
	}
}
//Turns 8 bits to 1 byte
char F8bitsto1B(int* bits) {
	int sum = 0;
	for (int i = 0; i < 8; i++) {
		sum += bits[7 - i] * pow(2, i);
	}
	char asc = (unsigned char)sum;
	return asc;
}
//Turns 504 bits to 63 bytes
void F504bto63B(int* src, char* out) {
	int bits[8];
	for (int i = 0; i < 63; i++) {
		for (int j = 0; j < 8; j++) {
			bits[j] = src[i * 8 + j];
		}
		out[i] = F8bitsto1B(bits);
	}
}
//Turns 63 bytes to 504 bits
void F63Bto504b(char* src, int* out) {
	int bits[8];
	for (int i = 0; i < 63; i++) {
		F1Bto8bits(src[i], bits);
		for (int j = 0; j < 8; j++) {
			out[i * 8 + j] = bits[j];
		}
	}
}
//Disrupts the message and returns the number of flipped bits
int disrMsg(char* msg, double chance) {
	int bitRep[504], cnt = 0;
	double rnd;
	F63Bto504b(msg, bitRep);
	for (int i = 0; i < 504; i++) {
		rnd = (double)rand() / (RAND_MAX);
		if (rnd < chance) {
			bitRep[i] = (bitRep[i] + 1) % 2;
			cnt++;
		}
	}
	F504bto63B(bitRep, msg);
	return cnt;
}

int main(int argc, char **argv) //senderPort7891 recieverPort chance int-seed
{
	int senderPort = atoi(argv[1]), recieverPort = atoi(argv[2]), randSeed = atoi(argv[4]);
	double chance;
	sscanf(argv[3], "%lf", &chance);
	// Initialize windows networking
	srand(randSeed);
	int received, sent, flpbits = 0, bytes = 0;
	char ipR[32], ipS[32];
	char recieved_buf[63], fb[256];
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR)
		printf("Error at WSAStartup()\n");
	struct sockaddr_in senderAdd, recAdd;
	// Create the socket that will listen for sender and receiver
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	SOCKET r = socket(AF_INET, SOCK_STREAM, 0);
	senderAdd.sin_family = AF_INET;
	senderAdd.sin_addr.s_addr = INADDR_ANY;
	senderAdd.sin_port = htons(senderPort);
	recAdd.sin_family = AF_INET;
	recAdd.sin_addr.s_addr = INADDR_ANY;
	recAdd.sin_port = htons(recieverPort);

	int status = bind(s, (SOCKADDR*)&senderAdd, sizeof(senderAdd));
	status = bind(r, (SOCKADDR*)&recAdd, sizeof(recAdd));
	status = listen(s, 2);
	status = listen(r, 2);
	// Reciever connected
	r = accept(r, (SOCKADDR*)&recAdd, NULL);
	// Sender connected
	s = accept(s, (SOCKADDR*)&senderAdd, NULL);
	// Start transfering disrupted messages
	while ((received = recv(s, recieved_buf, MSG_SIZE, 0)) > 0) {
		bytes += received;
		flpbits += disrMsg(recieved_buf, chance);
		sent = send(r, recieved_buf, MSG_SIZE, 0);
	}
	int z = 32, port;


	getpeername(s, (struct sockaddr*)&senderAdd, &z);

	// Get IP's, source:http://stackoverflow.com/questions/2064636/getting-the-source-address-of-an-incoming-socket-connection
	// deal with both IPv4 and IPv6:
	if (senderAdd.sin_family == AF_INET) {
		struct sockaddr_in *s = (struct sockaddr_in *)&senderAdd;
		port = ntohs(s->sin_port);
		inet_ntop(AF_INET, &s->sin_addr, ipS, sizeof ipS);
	}
	else { // AF_INET6
		struct sockaddr_in6 *s = (struct sockaddr_in6 *)&senderAdd;
		port = ntohs(s->sin6_port);
		inet_ntop(AF_INET6, &s->sin6_addr, ipS, sizeof ipS);
	}

	getpeername(r, (struct sockaddr*)&recAdd, &z);

	// deal with both IPv4 and IPv6:
	if (recAdd.sin_family == AF_INET) {
		struct sockaddr_in *r = (struct sockaddr_in *)&recAdd;
		port = ntohs(r->sin_port);
		inet_ntop(AF_INET, &r->sin_addr, ipR, sizeof ipR);
	}
	else { // AF_INET6
		struct sockaddr_in6 *r = (struct sockaddr_in6 *)&recAdd;
		port = ntohs(r->sin6_port);
		inet_ntop(AF_INET6, &r->sin6_addr, ipR, sizeof ipR);
	}




	// Sender has finished sending, stop sending to receiver
	status = shutdown(r, 1);
	// Get feedback from reciever and send to sender
	received = recv(r, fb, 256, 0);
	sent = send(s, fb, 256, 0);
	// Close sockets and print details
	status = shutdown(r, 2);
	status = shutdown(s, 2);
	fprintf(stderr, "sender: %s\nreceiver: %s\n%d bytes flipped %d bits\n",ipS,ipR,bytes,flpbits);
	return 0;
}