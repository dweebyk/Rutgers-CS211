#ifndef _CALC_H_
#define _CALC_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define DECIMAL 'd'
#define BINARY 'b'
#define OCTAL 'o'
#define HEX 'x'




char * initializeInput(char * number);

/*Functions for turning input into two's complement binary*/
char * binaryIntoTwoComp(char* input, int end);
char * octalIntoTwoComp(char* input, int end);
char * decimalIntoTwoComp(char* input, int end);
char * hexIntoTwoComp(char* input, int end);

/*Arithmetic operations and various basic repetitive 
* computation functions declared in helper.h and 
* defined in helper.c */


/*Output functions*/
void spitBinary(char * bitstring);
void spitOctal(char * bitstring);
void spitHex(char * bitstring);

bool isInvalid(int argc, char *argv[]);




#endif /* _CALC_H_ */
