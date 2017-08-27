#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "formula.h"
#include <ctype.h>
#include "nCr.h"
#include <sys/time.h>



bool
checkHelp(char * input)
{
	if(strcmp(input, "-h") == 0)
	{	return true;	}

	return false;
}


bool
isNegative(char * argv[])
{
	return argv[1][0] == '-';
}

bool 
isInvalidInput(int argc, char * argv[])
{
	int		i = 1;		//variable for iterating through input

	//check if only one argument was entered
	if (argc < 2)
	{
		printf("No number was input.  Exiting now.\n\n");
		return true;
	}


	if (argc > 2)
	{
		printf("Too many arguments were entered.  Exiting now.\n\n");
		return true;
	}


	//case for only one character
	if (isdigit(argv[1][0]) == 0 && strlen(argv[1]) == 1) 
	{
		printf("A single non-numerical character was entered.  Exiting now.\n\n");
	       	return true;
	}



	
	if (argv[1][0] == '-' && strlen(argv[1]) > 1)
	{
		//display helpflag
		if (checkHelp(argv[1]))
		{
			printf("\n**********************************************************************************************************\n\n");
			printf("Outputs (1 + x)^n as an expanded polynomial, where n is input by the user.\nFormat for input: ./formula <integer>\nNegative values will be ignored.\nWARNING: Integers over 12 cause overlow, and subsequently the coefficients will be returned as 0!\n\n");	
			printf("**********************************************************************************************************\n\n");
			return true;
		}

		for (; i < strlen(argv[1]); i++)
		{	
			if (isdigit(argv[1][i]) == 0)
			{
			printf("The number you entered contains non-numerical characters in its digits.  Exiting now.\n\n");
			return true;
			}

		}

		i = 1;
	}


	for(; i < strlen(argv[1]); i++)
	{
		if (isdigit(argv[1][i]) == 0)
		{
			printf("The number you entered contains non-numerical characters in its digits.  Exiting now.\n\n");
			return true;
		}
	}

	return false;
}





void
displayFunction(int n)
{
	int		r = 1;
	int		coefficient = 0;
	bool		overflow = false;

	/*Even when n is 0, 1 is part of the function definition, as any number y ^ 0 = 1
	 * so (1 + x)^n must include 1 even when n = 0*/
	printf("1");


	for (; r <= n; r++)
	{
		coefficient = nCr(n, r);
		if (coefficient == 0)
		{  overflow = true; }
		printf(" + %d*x^%d", coefficient, r);	
	}

	if (overflow)
	{	printf("\n\nOverflow was detected.\n"); }

}




int 
main (int argc, char * argv[])
{
	int		n; //this n is from (1 + x)^n
	bool		negative;
	struct		timeval start, end;

	/*Error checking on input*/
	if (isInvalidInput(argc, argv))
	{
		return 0;
	}

	else
	{

		gettimeofday(&start, NULL);
		negative = isNegative(argv);

		if (negative)
		{
			n = atoi(argv[1] + 1);
			printf("(1 + x)^-%d = ", n);
			printf("1 / ( ");
		}
		else 
		{ 
		 	n = atoi(argv[1]);
			printf("(1 + x)^%d = ", n);
		}

		displayFunction(n);

		if (negative)
		{
			printf(" )");
		}

		gettimeofday(&end, NULL);
		printf("\nTime required: %ld milliseconds\n\n",((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));
	
	}	



	return 0;

}

