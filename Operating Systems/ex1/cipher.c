#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#define SIZ 99
int crypt (char* srcAdd, char* keyAdd, char* destAdd) {
unsigned char flBuff[SIZ], keyBuff[SIZ], destBuff[SIZ];
int flChunkSize = 0, keyPos = 0, flPos = 0;
int fl, key, dest, keylength;
fl = open(srcAdd,O_RDONLY,0777);
if (fl < 0)
return 1;
key = open(keyAdd,O_RDONLY,0777);
if (key < 0)
return 2;
keylength = lseek(key, 0, SEEK_END);
lseek(key, 0, SEEK_SET);
dest = open(destAdd,O_WRONLY,0777);
if (dest < 0){
dest = open(destAdd,O_CREAT|O_WRONLY,0777);
}
while (read(fl,flBuff,SIZ)) {
flChunkSize = lseek(fl, 0, SEEK_CUR) - flPos;
read(key,keyBuff,SIZ);
for (int i = 0; i < flChunkSize; i++) {
if (keyPos >= keylength) {
lseek(key,0,SEEK_SET);
read(key,keyBuff,flChunkSize-i);
keyPos = 0;
}
destBuff[i] = flBuff[i] ^ keyBuff[keyPos];
flPos++; keyPos++;
}
write(dest,destBuff,flChunkSize);
}
close(fl); close(key); close(dest);
return 0;
}

int main (int argc, char** argv) {
char destAdd[128], srcAdd[128];
DIR* srcFolder = opendir(argv[1]);
DIR* destFolder = opendir(argv[3]);
int problem = 0;
int key = open(argv[2],O_RDONLY);
if (key < 0) {
printf("Can't find key file\n");
close(key);
closedir(srcFolder);
closedir(destFolder);
return 0;
}
close(key);
if (!srcFolder) {
printf("Can't find source folder\n");
close(key);
closedir(srcFolder);
closedir(destFolder);
return 0;
}
if (!destFolder) {
problem = mkdir(argv[3],0777);
if (problem == -1) {
printf("Error occured while making directory");
return 1;
}
}
struct dirent *dfold;
while ((dfold = readdir(srcFolder)) != NULL) {
if (strcmp(dfold->d_name,".") && strcmp(dfold->d_name,"..")) {
strcpy(destAdd,argv[3]);
strcat(destAdd,"/");
strcat(destAdd,dfold->d_name);
strcpy(srcAdd,argv[1]);
strcat(srcAdd,"/");
strcat(srcAdd,dfold->d_name);
problem = crypt(srcAdd,argv[2],destAdd);
if (problem == 1) {
printf("Error occured while openning file");
return 1;
}
if (problem == 2) {
printf("Error occured while openning key file");
return 1;
}
}
}
closedir(destFolder);
closedir(srcFolder);
return 0;
}
