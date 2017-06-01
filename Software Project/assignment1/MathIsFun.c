#include "MathIsFun.h"

/*
 * Calculates the largest integer less or equal than the square root of x.
 * funSqrt(10) = 3
 * funSqrt(16) = 4
 * @param x - An integer for which the function applies
 * @return the value of |_sqrt(x)_|, if x is negative then the function
 * returns -1.
 */
int funSqrt(int x); // ==> Declaration is always in the beginning of the file.

int mod (int x, int d) {
	if (x >= 0) {
		return x%d;
	}
	else {
		return (x%d)+d;
	}
}

int funPow(int x, int n, int d) {
	if (d == 1) {
		return 0;
	}
	if (n == 0) {
		return 1;
	}
	if (n%2 == 0) {
		return mod((funPow(x,n/2,d)*funPow(x,n/2,d)),d);
	}
	else {
		return mod(mod(x,d)*funPow(x,n/2,d)*funPow(x,n/2,d),d);
	}
}

int funSqrt(int x) {
	if(x<0){
		return -1;
	}
	int left = 0;
	int right = x;
	int i = (right + left)/2;
	while (left <= right){
		if (i*i <= x && x < (i+1)*(i+1)){
			return i;
		}
		if (i*i > x){
			right = i - 1;
		}
		if(x >= (i+1)*(i+1)){
			left = i + 1;
		}
		i = (right + left)/2;
	}
	return -1;
}

bool funPrimeCheck(int x) {
	//Your implementation
	//Declaration + Variable initializing at the beginning of the function
	if(x<0){
		return 0;
	}
	if(x==1 || x == 0){
		return false;
	}
	for(int i=2; i<=funSqrt(x); i++){
		if(x%i == 0){
			return false;
		}
	}
	return true;
}

bool funPalindromeCheck(int x) {
	int num = x;
	if(x<0){
		return 0;
	} else if(x==0){
		return true;
	}
	int length = 0;
	while(x>0){
		x/=10;
		length++;
	}
	int digits[length];
	int reverse[length];
	for( int i=0; i<length; i++){
		digits[i] = reverse[length-1-i] = num % 10;
		num /= 10;
	}
	for( int i=0; i<length; i++){
		if(digits[i] != reverse[i]){
			return false;
		}
	}
	return true;
}
