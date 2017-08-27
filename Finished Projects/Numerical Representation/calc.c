#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "calc.h"
#include "helper.h"



/*At this point, it is assumed that the number struct has an inputString provided*/
char * 
initializeInput(char * input)
{
	int		start = 0; //here, start indicates the index at which the base is given
	char		base;
	bool		negative = false;	
	char *		decimalbitstring;
	char *		errorstring = createEmptyBitstring(64);

	if (input[0] == '-')
	{
		negative = true;
		start = 1;
	}

	base = input[start];

	switch(base)
	{
		case DECIMAL:
			decimalbitstring = decimalIntoTwoComp(input, start);
			if (negative)
			{ decimalbitstring = findNegative(decimalbitstring, 64); }
		
			return decimalbitstring;

			break;
		case BINARY:
			return binaryIntoTwoComp(input, start);
			break;
		case OCTAL:
			return octalIntoTwoComp(input, start);
			break;
		case HEX:
			return hexIntoTwoComp(input, start);
			break;
		default:
			fprintf(stderr, "Unrecognized base.\nResults are now invalid.\n");
			return errorstring;
			break;
	}

}







char * 
octalIntoTwoComp(char * input, int end)
{
	char *		bitstring = createEmptyBitstring(64);
	int 		start = strlen(input) - 1;
	int 		bitposition = 63;
	int 		loopCount = 0;



	/*Groups of 3 bits do not cleanly divide into 64.  We were told to treat
	 *the last character as a set either of 000 or 001.  That means the only possible
	 *leading character in a maximally large octal is either 0 or 1*/
	for (; start > end; start--, bitposition -= 3, loopCount++)
	{
		if (loopCount == 21)
		{
			/*Treat the first number in a maximally large oct 
			 * bunch of bits that are either 000 or 001.  However,
			 * I am only going to record the low bit*/
			bitstring[0] = (char)(input[end + 1]);
		}

		else
		{	 
			switch (input[start])
			{
				case '0':
					bitstring[bitposition] = '0';
					bitstring[bitposition - 1] = '0';
					bitstring[bitposition - 2] = '0';
					break;

				case '1':
					bitstring[bitposition] = '1';
					bitstring[bitposition - 1] = '0';
					bitstring[bitposition - 2] = '0';
					break;

				case '2':
					bitstring[bitposition] = '0';
					bitstring[bitposition - 1] = '1';
					bitstring[bitposition - 2] = '0';
					break;

				case '3':
					bitstring[bitposition] = '1';
					bitstring[bitposition - 1] = '1';
					bitstring[bitposition - 2] = '0';
					break;

				case '4':
					bitstring[bitposition] = '0';
					bitstring[bitposition - 1] = '0';
					bitstring[bitposition - 2] = '1';
					break;

				case '5':
					bitstring[bitposition] = '1';
					bitstring[bitposition - 1] = '0';
					bitstring[bitposition - 2] = '1';
					break;

				case '6':
					bitstring[bitposition] = '0';
					bitstring[bitposition - 1] = '1';
					bitstring[bitposition - 2] = '1';
					break;

				case '7':
					bitstring[bitposition] = '1';
					bitstring[bitposition - 1] = '1';
					bitstring[bitposition - 2] = '1';
					break;
				default:

					fprintf(stderr, "Unrecognized numeral in oct string: %c.\nResults are now invalid.\n", input[start]);

				break;
			}
		}

	}

	return bitstring;

}





char* 
decimalIntoTwoComp(char * input, int end)
{
	char * 		bitstring = createEmptyBitstring(64);
	int 		bitposition = 63;
	int 		start = strlen(input) - 1;
	long long int 		numeral = 0;
	long long int 		base = 0;
	long long int 	decimal = 0;
	char 		adder = '0';


	/*First we need to parse an int out of the string*/
	for (; start > end; start--, base++)
	{
		numeral = (long long int )charToInt(input[start]);
		decimal += (numeral * intPow((long long int)10, base));		
	}


	while (decimal != 0)
	{
		adder = (char)(0x30 + (decimal % 2));
		bitstring[bitposition] = adder;
		decimal /= 2;
		bitposition--;
	}


	return bitstring;

}





char* 
hexIntoTwoComp(char * input, int end)
{
	char *		bitstring = createEmptyBitstring(64);

	int 		start = strlen(input) - 1;
	int 		bitposition = 63;

	/*4 bits divides cleanly into 64, so we don't have to check if we're at 
	 * any particular bit that would be left behind*/
	for (; start > end; start--, bitposition -= 4)
	{		
		switch(input[start])
		{

			case '0':
				bitstring[bitposition] = '0';
				bitstring[bitposition - 1] = '0';
				bitstring[bitposition - 2] = '0';
				bitstring[bitposition - 3] = '0';
				break;

			case '1':
				bitstring[bitposition] = '1';
				bitstring[bitposition - 1] = '0';
				bitstring[bitposition - 2] = '0';
				bitstring[bitposition - 3] = '0';
				break;


			case '2':
				bitstring[bitposition] = '0';
				bitstring[bitposition - 1] = '1';
				bitstring[bitposition - 2] = '0';
				bitstring[bitposition - 3] = '0';
				break;


			case '3':
				bitstring[bitposition] = '1';
				bitstring[bitposition - 1] = '1';
				bitstring[bitposition - 2] = '0';
				bitstring[bitposition - 3] = '0';
				break;

				
			case '4':
				bitstring[bitposition] = '0';
				bitstring[bitposition - 1] = '0';
				bitstring[bitposition - 2] = '1';
				bitstring[bitposition - 3] = '0';
				break;



			case '5':
				bitstring[bitposition] = '1';
				bitstring[bitposition - 1] = '0';
				bitstring[bitposition - 2] = '1';
				bitstring[bitposition - 3] = '0';
				break;


			case '6':
				bitstring[bitposition] = '0';
				bitstring[bitposition - 1] = '1';
				bitstring[bitposition - 2] = '1';
				bitstring[bitposition - 3] = '0';
				break;

			case '7':
				bitstring[bitposition] = '1';
				bitstring[bitposition - 1] = '1';
				bitstring[bitposition - 2] = '1';
				bitstring[bitposition - 3] = '0';
				break;

			case '8':
				bitstring[bitposition] = '0';
				bitstring[bitposition - 1] = '0';
				bitstring[bitposition - 2] = '0';
				bitstring[bitposition - 3] = '1';
				break;

			case '9':
				bitstring[bitposition] = '1';
				bitstring[bitposition - 1] = '0';
				bitstring[bitposition - 2] = '0';
				bitstring[bitposition - 3] = '1';
				break;

			case 'A':
			case 'a':
				bitstring[bitposition] = '0';
				bitstring[bitposition - 1] = '1';
				bitstring[bitposition - 2] = '0';
				bitstring[bitposition - 3] = '1';
				break;

			case 'B':
			case 'b':
				bitstring[bitposition] = '1';
				bitstring[bitposition - 1] = '1';
				bitstring[bitposition - 2] = '0';
				bitstring[bitposition - 3] = '1';
				break;

			case 'C':
			case 'c':
				bitstring[bitposition] = '0';
				bitstring[bitposition - 1] = '0';
				bitstring[bitposition - 2] = '1';
				bitstring[bitposition - 3] = '1';
				break;

			case 'D':
			case 'd':
				bitstring[bitposition] = '1';
				bitstring[bitposition - 1] = '0';
				bitstring[bitposition - 2] = '1';
				bitstring[bitposition - 3] = '1';
				break;

			case 'E':
			case 'e':
				bitstring[bitposition] = '0';
				bitstring[bitposition - 1] = '1';
				bitstring[bitposition - 2] = '1';
				bitstring[bitposition - 3] = '1';
				break;


			case 'F':
			case 'f':
				bitstring[bitposition] = '1';
				bitstring[bitposition - 1] = '1';
				bitstring[bitposition - 2] = '1';
				bitstring[bitposition - 3] = '1';
				break;

			default:
				printf("Unrecognized numeral in hex string: %c.\nResults are now invalid.\n", input[start]);
				break;
		}


	}

	return bitstring;

}




/*Specific too 64 bits*/
char* 
binaryIntoTwoComp(char * input, int end)
{
	/*pretty sure it's all 0, but this is just to be safe*/
	char * 		bitstring = createEmptyBitstring(64);
	int 		start = strlen(input) - 1;
	int 		bitposition = 63;

	/*we're reading from the end of the string to the beginning, so 
	 * the loop invariant is that start > end*/
	for (; start > end; start--, bitposition--)
	{
	       	bitstring[bitposition] = input[start];	
	}


	return bitstring;

}





void 
spitBinary(char * bitstring)
{
	printf("b%s\n", bitstring);
}



/*Specific to 64 bits*/
void 
spitOctal(char * bitstring)
{

	int		loopCount = 0;
	int 		value = 0;
	char * 		octChar;
	int 		start = 1;
	int 		i = 0;

	printf("o");


	if (bitstring[0] == '1')
	{
		printf("1");
	}
	else { printf("0"); }


	for (; loopCount < 21; start += 3, loopCount++)
	{
		for (; i < 3; i++)
		{
			value += (int)(( (int) (bitstring[start + i] - 0x30))*(intPow(2, 2-i)) );
		}

		octChar = malloc(sizeof(char));
		memset(octChar, decimalToHex(value), (sizeof(char)));

		printf("%s", octChar);
		free(octChar);
		value =  0;
		i = 0;
	}


	printf("\n");
}




/*Specific to 64 bits*/
void 
spitHex(char * bitstring)
{
	int 		loopCount = 0;
	char *	 	hexchar;
	int 		start = 0;
	int		value = 0;
	int		i = 3;

	printf("x");

	for(; loopCount < 16; start += 4, loopCount++)
	{

		for (; i >= 0; i--)
		{
			value += (int)((bitstring[start + i] - 0x30)*(intPow(2, 3-i)));
		}

		
		/*I kept getting errors when storing this all in a new array
		 * so as a workaround, I just malloc'd a string of one char
		 * so it would work with %s ;^) */
		hexchar = malloc(sizeof(char));
		memset(hexchar, decimalToHex(value), (sizeof(char)));

		printf("%s", hexchar);
		free(hexchar);
		value = 0;
		i = 3;

	}

	printf("\n");
}


char *
multiply(char* first, char* second)
{
	long long int		base = 0;
	bool			negative1 = (first[0] == '1');
	bool			negative2 = (second[0] == '1');
	long long int		value = 0;
	long long int		firstNum = 0; 
	long long int		secondNum = 0; 	
	long long int		result = 0;
	int			i = 63;	
	char			adder = '0';
	int			negMul = 1;
	char *			resultBin = createEmptyBitstring(64);
	

	if (negative1)
	{
		first = findNegative(first, 64);
		negMul *= -1;
	}
	
	if (negative2)
	{
		second = findNegative(second, 64);
		negMul *= -1;
	}

	
	for (; i >= 0; i--, base++)
	{
		value = (long long int) (first[i] - 0x30); //gives us int value
		firstNum += (value * intPow((long long int)2, base));
	}
	
	i = 63;
	base = 0;
	value = 0;
	
	for (; i >= 0; i--, base++)
	{
		value = (long long int) (second[i] - 0x30); //gives us int value
		secondNum += (value * intPow(2, base));
	}

	result = firstNum * secondNum * negMul;
	i = 63;
	
	if (result < 0)
	{
		result *= -1;		
		
		while (result != 0)
		{
			adder = (char)(0x30 + (result % 2));
			resultBin[i] = adder;
			result /= 2;
			i--;
		}	
		
		return findNegative(resultBin, 64);
	}
	
	else if (result > 0)
	{
		while (result != 0)
		{
			adder = (char)(0x30 + (result % 2));
			resultBin[i] = adder;
			result /= 2;
			i--;
		}
		
		return resultBin;
	}
	
	else { return createEmptyBitstring(64); }
}



/*Checks for long input and mismatching amounts of arguments
 * Later switch statements handle validity of numbers themselves*/
bool
isInvalid(int argc, char *argv[])
{
	int		i = 0;		//basic iterator

	 if (argc > 5)
	{
	       	printf("Too many arguments.\n");
		return true;
	}
	
	if (argc < 5)
	{
	       	printf("Too few arguments.\n");
		return true;
	}
	


	for (; i < 2; i++)
	{
		if (argv[2 + i][0] == 'd' || (argv[2 + i][0] == '-' && argv[2 + i][1] == 'd'))
		{
			if (strlen(argv[2+i]) > 22)
			{
				printf("Decimal input is too large for 64 bits.\n");
				return true;
			}
		}	



		if (argv[2 + i][0] == 'x')
		{
			if (strlen(argv[2+i]) > 17)
			{
				printf("Hex input is too large for 64 bits.\n");
				return true;
			}
		}	


		if (argv[2 + i][0] == 'o')
		{
			if (strlen(argv[2+i]) > 24)
			{
				printf("Octal input is too large for 64 bits.\n");
				return true;
			}
		}	



		if (argv[2 + i][0] == 'b')
		{
			if (strlen(argv[2+i]) > 66)
			{
				printf("Binary input is too large for 64 bits.\n");
				return true;
			}
		}	
	}
	
	

	return false;
}


int 
main(int argc, char *argv[])
{
	/*checking for invalid amount of inputs*/
	if (isInvalid(argc, argv))
	{
		return 0;
	}

	else
	{

		/*relevant variables here*/
		char 		operation;
		char *		first;
		char *		input1;
		char *		input2;
		char *		second;
		char *		outputBitString;
		char		outputBase;
		


		/*record operation here (+, -, mul)*/
		if (strlen(argv[1]) == 0)
		{
			operation = '*';
		}

		else
		{
			operation = argv[1][0];
		}


	
		/*record both numbers here*/
		input1  = strdup(argv[2]);
		input2  = strdup(argv[3]);

		/*recording outputbase here*/	
		outputBase = argv[4][0];

		/*initialize here*/
		first = initializeInput(input1);
		second = initializeInput(input2);

	
		/*Compute sum, sub, or mul here*/
		switch (operation)
		{
			case '+':
				outputBitString = add(first, second, 64);
				break;
			case '-':
				outputBitString = subtract(first, second, 64);
				break;
			case '*':
				outputBitString = multiply(first, second);
				break;
			default:
				printf("Invalid operation.\n");
				break;
	
		}
	
		/*Spit out number here*/
		switch (outputBase)
		{
			case DECIMAL:
				spitDecimal(outputBitString, 64, true);
				break;
	
			case HEX:
				spitHex(outputBitString);
				break;
	
			case OCTAL:
				spitOctal(outputBitString);
				break;
	
			case BINARY:
				spitBinary(outputBitString);
				break;
			default:
				printf("%c is not a valid base.", outputBase);
				break;
	
		}
	}
	
	return 0;
}
