/*
 * main.c
 *
 *  Created on: Mar 21, 2016
 *      Author: project
 */

#include <stdio.h>
#include "MathIsFun.h"

int main() {
	int op;
	printf(
	"Welcome to Math Is Fun - beta version\n"
	"Supported operations are:\n"
	"1 - Power Calculation\n"
	"2 - Prime Check\n"
	"3 - Palindrome Check\n"
	"Please enter operation number (1/2/3): \n");
	scanf("%d",&op);
	int num;
	int exp;
	int modulu;
	int res;
	switch(op) {
	   case 1  :
		  printf("Please enter three space separated numbers: \n");
		  scanf("%d%d%d",&num,&exp,&modulu);
		  printf("res = %d\n",funPow(num,exp,modulu));
	      break;
	   case 2  :
		  printf("Please enter an integer: \n");
		  scanf("%d",&num);
		  res = funPrimeCheck(num);
		  if(res == 1){
			  printf("res = true\n");
		  } else{
			  printf("res = false\n");
		  }
	      break;
	   case 3:
		   printf("Please enter an integer: \n");
		   scanf("%d",&num);
		   res = funPalindromeCheck(num);
		   if(res == 1){
			   printf("res = true\n");
		   } else{
			   printf("res = false\n");
		   }
		   break;
	}
	return 0;
}
