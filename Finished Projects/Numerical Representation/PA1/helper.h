#ifndef _HELPER_H_
#define _HELPER_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


/*Used to compute values between two bitstrings*/
char * add(char * first, char * second, int size);
char * findNegative(char * bitstring, int size);
char * subtract(char * first, char * second, int size);


/*Used to display given a bitstring*/
void spitDecimal(char * bitstring, int size, bool drawD);

/*Used to convert values across data types*/
char decimalToHex(int num);
char charToInt(char num);

/*Verification for validating input*/
bool isHex(char hex);
bool isOct(char oct);
bool isDec(char oct);


/*I created a LOT of new empty strings, so this just expedited the declaration speed*/
char * createEmptyBitstring(int size);

/*Didn't want to deal with rounding errors with double, so I wrote a small integer pow function*/
long long int intPow(long long int num, long long int power);



#endif /* _HELPER_H_ */

