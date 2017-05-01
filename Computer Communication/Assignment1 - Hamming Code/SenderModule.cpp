// SenderModule.cpp : Defines the entry point for the console application.
//

#include <stdafx.h>
#include <cmath>
#include <winsock2.h> 
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")
#define CHUNK_SIZE 57
#define MSG_SIZE 63

//Convert number to its 8 bit representaion
void numTo8Bit(int* bits, int num) {
	int x;
	for (int i = 0; i < 8; i++) {
		x = num % 2;
		bits[7-i] = x;
		num = num / 2;
	}
}
//Check if a number is a power of 2 in required range
int Pow2(int x) {
	return (x == 1 || x == 2 || x == 4 || x == 8 || x == 16 || x == 32);
}
//Turns byte to 8-bit representaion (in int array)
void F1Bto8bits(char byte, int* bits) {
	int msk = 1;
	for (int i = 0; i < 8; i++) {
		if (byte & msk)
			bits[7-i] = 1;
		else
			bits[7-i] = 0;
		msk = msk << 1;
	}
}
//Turns 8 bits to 1 byte
char F8bitsto1B(int* bits) { 
	int sum = 0;
	for (int i = 0; i < 8; i++) {
		sum += bits[7-i] * pow(2, i);
	}
	char asc = (unsigned char)sum;
	return asc;
}
//Turns 57 bytes to 456 bits (in int array)
void F57Bto456b(char* src, int* out) { 
	int bits[8];
	for (int i = 0; i < 57; i++) {
		F1Bto8bits(src[i], bits);
		for (int j = 0; j < 8; j++) {
			out[i * 8 + j] = bits[j];
		}
	}
}
//Turns 504 bits to 63 Bytes
void F504bto63B(int* src, char* out) { 
	int bits[8];
	for (int i = 0; i < 63; i++) {
		for (int j = 0; j < 8; j++) {
			bits[j] = src[i * 8 + j];
		}
		out[i] = F8bitsto1B(bits);
	}
}
//Fill hamming code bits in 504 bit int array in a given 2^x-1 position which is 0 before calling
void fillHamBits(int* src, int pos) {
	int* baseAdd = src;
	int pos1 = pos + 1, chk = 0, xor = 0;
	int bits1[8], bits2[8];
	numTo8Bit(bits1, pos1); //bits1 now contains the bit rep of the given position
	for (int z = 0; z < 8; z++) {
		for (int i = 0; i < 63; i++) {
			numTo8Bit(bits2, i + 1);
			//Check if pos and number have some '1' bit in common in bit-rep
			for (int j = 0; j < 8; j++) {
				if (bits1[j] + bits2[j] == 2) {
					chk = 1;
					break;
				}
				else
					chk = 0;
			}
			if (chk)
				xor += baseAdd[i];
		}
		baseAdd[pos] = xor % 2;
		xor = 0;
		baseAdd += 63;
	}
	
}
//Turns 57 bytes to 63 bytes with hamming coding
void ham57to63(char* src, char* out) {
	int bitrep[456], hammed[504];
	int* baseAdd1 = bitrep;
	int* baseAdd2 = hammed;
	F57Bto456b(src,bitrep);
	int cnt = 0;
	//Fill "hammed" with data bits
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 57; j++) {
			if (!Pow2(cnt + 1))
				baseAdd2[cnt] = baseAdd1[j];
			else {
				j--;
				baseAdd2[cnt] = 0;
			}
			cnt++;
		}
		cnt = 0;
		baseAdd1 += 57;
		baseAdd2 += 63;
	}
	//Fill "hammed" with hamming code bits
	for (int i = 1; i < 33; i = i * 2)
		fillHamBits(hammed, i - 1);
	//Now just use F504bto63B
	F504bto63B(hammed, out);
	return;
}

int main(int argc, char **argv)
{
	int read, rec, reconstructed, corrected, status, sent, received, sentCnt = 0, port;
	port = atoi(argv[2]);
	FILE* file;
	char readBuf[CHUNK_SIZE], msgBuf[MSG_SIZE], fb[256];
	file = fopen(argv[3], "rb");
	if (!file) {
		fprintf(stderr, "Can't open file\n");
		exit(1);
	}
	// Initialize windows networking
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR)
		printf("Error at WSAStartup()\n");
	// Create the socket that will listen for incoming TCP connections
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in chanAdd;
	chanAdd.sin_family = AF_INET;
	chanAdd.sin_addr.s_addr = inet_addr(argv[1]);
	chanAdd.sin_port = htons(port);
	status = connect(s, (SOCKADDR*)&chanAdd, sizeof(chanAdd));
	// Read file chunks and send them after hamming
	while (fread(readBuf, 1, CHUNK_SIZE, file) == CHUNK_SIZE) {
		ham57to63(readBuf, msgBuf);
		sent = send(s, msgBuf, MSG_SIZE, 0);
		sentCnt++;
	}
	// Stop sending and can only receive
	status = shutdown(s, 1);
	received = recv(s, fb, 256, 0);
	status = fclose(file);
	status = shutdown(s, 2);
	// Print details
	sscanf(fb, "%d %d %d ", &rec, &reconstructed, &corrected);
	fprintf(stderr, "received: %d bytes\nreconstructed: %d bytes\ncorrected: %d errors\n",rec, reconstructed,corrected);
	return 0;
}

