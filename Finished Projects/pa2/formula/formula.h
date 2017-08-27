#ifndef _FORMULA_H_
#define _FORMULA_H_


#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <sys/time.h>


/*Checks for the help flag*/
bool checkHelp(char * input);

/*Takes input as arguments like main, but STRICTLY
 * does not alter them.  Instead, it just checks them.
 * It also considers -h invalid, but prints a message
 * and waits for the user to input a valid number*/
bool isInvalidInput(int argc, char * argv[]);

/*Assumes valid input*/
bool isNegative(char * argv[]);

/*n is the maximum degree of the polynomial
 * described by (1 + n)^n*/
void displayFunction(int n);



#endif	/* _FORMULA_H_ */
