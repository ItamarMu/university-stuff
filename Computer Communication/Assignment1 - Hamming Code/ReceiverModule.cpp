// ReceiverModule.cpp : Defines the entry point for the console application.
//

#include <stdafx.h>
#include <cmath>
#include <winsock2.h> 
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")
#define MSG_SIZE 63
#define CHUNK_SIZE 57

//Convert number to its 8 bit representaion
void numTo8Bit(int* bits, int num) {
	int x;
	for (int i = 0; i < 8; i++) {
		x = num % 2;
		bits[7 - i] = x;
		num = num / 2;
	}
}
//Return the value of 8 bit number in decimal
int F8BitToNum(int* bits) {
	int sum = 0;
	for (int i = 0; i < 8; i++) {
		sum += bits[7 - i] * (int)pow(2, i);
	}
	return sum;
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
//Turns 57 bytes to 456 bits (in int array)
void F63Bto504b(char* src, int* out) {
	int bits[8];
	for (int i = 0; i < 63; i++) {
		F1Bto8bits(src[i], bits);
		for (int j = 0; j < 8; j++) {
			out[i * 8 + j] = bits[j];
		}
	}
}
//Translating hamming 504 bits to 456 bits
void decHam(int* src, int* out) {
	int* baseAdd1 = src;
	int* baseAdd2 = out;
	int cnt = 0;
	for (int j = 0; j < 8; j++) {
		for (int i = 0; i < 63; i++) {
			if (!Pow2(i + 1))
				baseAdd2[cnt++] = baseAdd1[i];
		}
		cnt = 0;
		baseAdd1 += 63;
		baseAdd2 += 57;
	}
}
//Turns 456 bits to 57 Bytes
void F456bto57B(int* src, char* out) {
	int bits[8];
	for (int i = 0; i < 57; i++) {
		for (int j = 0; j < 8; j++) {
			bits[j] = src[i * 8 + j];
		}
		out[i] = F8bitsto1B(bits);
	}
}
//Translate hammed message to original data
void ham63ToData57(char* src, char* out) {
	int bitrep[456], hammed[504];
	F63Bto504b(src, hammed);
	decHam(hammed, bitrep);
	F456bto57B(bitrep, out);
}
//Returns the xor value for given 63 bits for certain hamming code bit position
int xorVal(int* src, int hamPos) {
	int bits1[8], bits2[8], xor = 0, chk = 0;
	numTo8Bit(bits1, hamPos);
	for (int i = 0; i < 63; i++) {
		numTo8Bit(bits2, i + 1);
		for (int j = 0; j < 8; j++) {
			chk = bits1[j] + bits2[j];
			if (chk == 2)
				break;
		}
		if (chk == 2) {
			xor = (xor +src[i]) % 2;
		}
	}
	return xor;
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
//Fixes the message with hamming code bits, returns number of errors found
int fixHam(char* msg) {
	int bitRep[504], pos = 0, errorCnt = 0;
	int errorPosBits[8] = { 0,0,0,0,0,0,0,0 };
	F63Bto504b(msg, bitRep);
	int* baseAdd = bitRep;
	for (int j = 0; j < 8; j++) {
		for (int i = 0; i < 6; i++)
			errorPosBits[7 - i] = xorVal(baseAdd, (int)pow(2, i));
		pos = F8BitToNum(errorPosBits);
		if (pos > 0) {//Found Error
			baseAdd[pos - 1] = (baseAdd[pos - 1] + 1) % 2; //Fix error
			errorCnt++;
		}
		baseAdd += 63;
	}
	F504bto63B(bitRep, msg);
	return errorCnt;
}

int main(int argc, char **argv) //Args: Channel IP, Channel Port, File Name
{
	FILE* file;
	file = fopen(argv[3], "wb");
	if (!file) {
		fprintf(stderr, "Can't open file\n");
		exit(1);
	}
	// Initialize windows networking
	WSADATA wsaData;
	int status, sent, received, recCnt = 0, wrCnt = 0, fixCnt = 0, written, port = atoi(argv[2]);
	char recieved_buf[MSG_SIZE], writeBuf[CHUNK_SIZE], fb[256];
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR)
		printf("Error at WSAStartup()\n");
	// Create the socket that will listen for incoming TCP connections and another socket for incoming data
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in chanAdd;
	chanAdd.sin_family = AF_INET;
	chanAdd.sin_addr.s_addr = inet_addr(argv[1]);
	chanAdd.sin_port = htons(port);

	//status = bind(s, (SOCKADDR*)&chanAdd, sizeof(chanAdd));
	status = connect(s, (SOCKADDR*)&chanAdd, sizeof(chanAdd));
	status = listen(s, 2);
	// Start getting data and writing to file
	while ((received = recv(s, recieved_buf, MSG_SIZE, 0)) > 0) {
		recCnt += received;
		fixCnt += fixHam(recieved_buf);
		ham63ToData57(recieved_buf, writeBuf);
		written = fwrite(writeBuf, 1, CHUNK_SIZE, file); //after fix
		wrCnt += written;
	}
	fclose(file);
	snprintf(fb, 256, "%d %d %d ", recCnt, wrCnt, fixCnt);
	// Make details string and send back to channel and print them
	sent = send(s, fb, 256, 0);
	fprintf(stderr, "received: %d bytes\nwrote: %d bytes\ncorrected: %d errors\n", recCnt, wrCnt, fixCnt);
	return 0;
}
