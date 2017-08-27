#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "helper.h"




char * 
add(char * first, char * second, int size)
{
	char *		bitstring = createEmptyBitstring(size);
	int		carry = 0;
	int		i = size - 1;


	for (; i >= 0; i--)
	{
		/*all values are 1*/
		if (carry == 1 && first[i] == '1' && second[i] == '1')
		{	
			bitstring[i] = '1';
			carry = 1;
		}



		/*carry = 1 and only one of the bits is 1*/
		else if (carry == 1 && first[i] == '0' && second[i] == '1')
		{	
			bitstring[i] = '0';
			carry = 1;
		}

		/*carry = 1 and only one of the bits is 1*/
		else if (carry == 1 && first[i] == '1' && second[i] == '0')
		{	
			bitstring[i] = '0';
			carry = 1;
		}




		/*carry = 0 and only one of the two bits is 1*/
		else if (carry == 0 && first[i] == '0' && second[i] == '1')
		{	
			bitstring[i] = '1';
			carry = 0;
		}

		else if (carry == 0 && first[i] == '1' && second[i] == '0')
		{	
			bitstring[i] = '1';
			carry = 0;
		}



		/*carry = 0 and both bits are 1*/
		else if (carry == 0 && first[i] == '1' && second[i] == '1')
		{	
			bitstring[i] = '0';
			carry = 1;
		}



		/*carry = 1 and both bits are 0*/
		else if (carry == 1 && first[i] == '0' && second[i] == '0')
		{	
			bitstring[i] = '1';
			carry = 0;
		}


		/*all values are 0*/
		else if (carry == 0 && first[i] == '0' && second[i] == '0')
		{	
			bitstring[i] = '0';
			carry = 0;
		}
	}

	return bitstring;
}



char * 
findNegative(char * bitstring, int size)
{
	/*flip bits*/
	int 		j = 0;
	char *		oneString = createEmptyBitstring(size);


	for (; j < size; j++)
	{
		if (bitstring[j] == '1') { bitstring[j] = '0'; }
		else { bitstring[j] = '1'; }
	}

	

	oneString[size - 1] = '1';

	return add(bitstring, oneString, size);	

}


char * 
subtract(char * first, char * second, int size)
{
	char *	negativeSecond = findNegative(second, size);
	return add(first, negativeSecond, size);
}







void 
spitDecimal(char * bitstring, int size, bool drawD)
{
	long long int		base = 0;
	char *			decChar;
	bool			negative = (bitstring[0] == '1');
	long long int		value = 0;
	long long int		decimal = 0; 
	int			i = size - 1;	
	int			j = 1;
	char			digit = '0';
	char *			backwardsNum = createEmptyBitstring(22);  //i'll change this value later to something more general
	int			finalstringlen = 0;


	/*First check if two's complement string is negative*/
	if (negative)
	{
		bitstring = findNegative(bitstring, size);
		printf("-");
	}

	if (drawD) //used only in calc function
	{
		printf("d");
	}	
	
	
	for (; i >= 0; i--, base++)
	{
		value = (long long int) (bitstring[i] - 0x30); //gives us int value
		decimal += (value * intPow((long long int)2, base));
	}


	if (decimal > 0)
	{

		while (decimal != 0)
		{
			digit = (char)(0x30 + decimal % 10);
			backwardsNum[finalstringlen] = digit;
			finalstringlen++;
			decimal /= 10;
		}

		for (; j <= finalstringlen; j++)
		{
			decChar = malloc(sizeof(char));
			memset(decChar, backwardsNum[(finalstringlen - j)], (sizeof(char)));
			printf("%s", decChar);
			free(decChar);
		}

	}

	else if (decimal < 0 && decimal != 0)
	{
		printf("This number switched signs in the middle of computation, signalling that there might have been overflow.  Results of decimal multiplication may not be entirely accurate.");
	}

	else { printf("0");	}

	printf("\n");

}




char 
decimalToHex(int num)
{
	switch (num)
	{
		case 0:
			return '0';
			break;

		case 1:
			return '1';
			break;

		case 2:
			return '2';
			break;

		case 3:
			return '3';
			break;

		case 4:
			return '4';
			break;

		case 5:
			return '5';
			break;

		case 6:
			return '6';
			break;

		case 7:
			return '7';
			break;

		case 8:
			return '8';
			break;

		case 9:
			return '9';
			break;

		case 10:
			return 'A';
			break;

		case 11:
			return 'B';
			break;

		case 12:
			return 'C';
			break;

		case 13:
			return 'D';
			break;

		case 14:
			return 'E';
			break;

		case 15:
			return 'F';
			break;
		default:
			printf("%i does not correspond with a character", num);
			break;
	}

	return ' ';
}



char 
charToInt(char num)
{
	switch (num)
	{
		case '0':
			return 0;
			break;

		case '1':
			return 1;
			break;
		case '2':
			return 2;
			break;

		case '3':
			return 3;
			break;

		case '4':
			return 4;
			break;

		case '5':
			return 5;
			break;

		case '6':
			return 6;
			break;

		case '7':
			return 7;
			break;

		case '8':
			return 8;
			break;

		case '9':
			return 9;
			break;
		default:
			fprintf(stderr,"charToInt - decimal character unrecgonized: %c\n", num);
			printf("charToInt - decimal character unrecgonized: %c\n", num);
			break;
	}


	return 0;

}

bool
isHex(char hex)
{
	return (hex == '0') ||  (hex == '1') || (hex == '2') || (hex == '3') ||
     (hex == '4') ||(hex == '5') ||(hex == '6') ||(hex == '7') ||(hex == '8') ||
     (hex == '9') ||(hex == 'A') ||(hex == 'a') ||(hex == 'B') ||(hex == 'b') ||
     (hex == 'C')||(hex == 'c')||(hex == 'D')||(hex == 'E')  ||(hex == 'e')||(hex == 'F')
  ||(hex == 'f');
}


bool
isOct(char oct)
{
	return (oct == '0') ||  (oct == '1') || (oct == '2') || (oct == '3') ||
	     (oct == '4') ||(oct == '5') ||(oct == '6') ||(oct == '7');

}


bool
isDec(char oct)
{
	return (oct == '0') ||  (oct == '1') || (oct == '2') || (oct == '3') ||
	            (oct == '4') ||(oct == '5') ||(oct == '6') ||(oct == '7')
		    ||(oct == '8')||(oct == '9');


}

long long int 
intPow(long long int num, long long int power)
{
	long long int	finalNum = 1;

	while (power > 0)
	{
		finalNum *= num;
		power--;
	}

	return finalNum;
}



char * 
createEmptyBitstring(int size)
{
	char * bitstring = malloc(sizeof(char)*size);
	memset(bitstring, '0', (sizeof(char)*size));
	return bitstring;
}

