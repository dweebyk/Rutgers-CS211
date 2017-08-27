#ifndef _FORMAT_H_
#define _FORMAT_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define NaN "NaN"
#define pinf "pinf"
#define ninf "ninf"
#define intInput "int"
#define floatInput "float"


/*This piece of code was given to us in class.  This will allow functions
 * to load bitstrings using bitshifting on the int and access on the float*/


/*Functions for loading bits and checking for NaN and inf*/
float 	getBitPattern(char * input);
bool	notNum(unsigned int exponent, unsigned int mantissa);
bool 	posinf(int sign, unsigned int exponent, unsigned int mantissa);
bool 	neginf(int sign, unsigned int exponent, unsigned int mantissa);



/*String and display functions*/
void 	floatToASCII(float x, char * output);
void 	displayFloat(char * input);



/*Function asked by floatToASCII to be defined*/
char *	intToDecASCII(int pow, char * exp);

/*Checking validity of input*/
bool isInvalid(int argc, char *argv[]);







#endif /*_FORMAT_H_*/
