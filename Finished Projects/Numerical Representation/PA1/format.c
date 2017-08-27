#include <stdlib.h> 
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "format.h"
#include "helper.h"



union Number {
	int i;
	float f;
};



bool 
notNum(unsigned int exponent, unsigned int mantissa)
{
	return (exponent == 255) && (mantissa != 0);
}



bool 
posinf(int sign, unsigned int exponent, unsigned int mantissa)
{
	return (sign == 0) && (exponent == 255) && (mantissa == 0);
}



bool 
neginf(int sign, unsigned int exponent, unsigned int mantissa)
{
	return (sign == -1) && (exponent == 255) && (mantissa == 0);
}




float 
getBitPattern(char * input)
{
	union Number	pattern;
	int		inputSize = 32;		//assumption: all valid input is 32 bits long
	int		shift = 0;
	
	pattern.i = 0;

	for (; shift < inputSize; shift++)
	{
		if (input[inputSize - (shift + 1)] == '1')
		{	pattern.i += (1 << shift); }
	}

	return pattern.f;
}




static void
get_float_digit( float x, char * digit, int * pow10, float * remainder )
{
	int			p10;

	if ( x == 0.0 )
	{
		*digit = '0';			// digit is always '0'
		*pow10 = 0;
		*remainder = 0.0;
	}
	else
	{
		*pow10 = 0;			// start with 10^0
		while ( x >= 10.0 )		// reduce
		{
			x /= 10.0;		// large values get smaller
			*pow10 += 1;
		}
		while ( x < 1.0 )		// increase
		{
			x *= 10.0;		// small values get larger
			*pow10 -= 1;
		}
		*digit = '0';
		do {				// 10.0 > x >= 1.0
			x -= 1.0;
			*digit += 1;		// digit is always non-'0'zero
		} while ( x >= 1.0 );
		p10 = 0;
		while ( p10 > *pow10 )		// leading digit is now removed from x
		{
			x /= 10;
			p10 -= 1;
		}
		while ( p10 < *pow10 )
		{
			x *= 10;
			p10 += 1;
		}
		*remainder = x;
	}
}



static void
append( char * s, char c )
{
	char		buf[2];

	buf[0] = c;
	buf[1] = '\0';
	strcat( s, buf );
}


/*Code provided by Professor Russell, modified slightly by me to print
* negative signs and the normalized exponent*/
void
floatToASCII( float x, char * output )
{
	char		c;
	int		pow10, p10, plast;
	int		i;
	float		remainder;
	char		exponent[10];
	union Number	a;
	unsigned int	biasedExp;
	unsigned int	mantissa;
	int		sign;

	a.f = x;
	biasedExp = a.i >> 23 & 0x000000ff;
	mantissa = a.i & 0x007fffff;
	sign = a.i >> 31;

	// +/- inf, +/- Nan stuff here
	if (notNum(biasedExp, mantissa) == true)
	{
		printf("NaN\n");
	}

	else if (posinf(sign, biasedExp, mantissa) == true)
	{
		printf("pinf\n");
	}

	else if (neginf(sign, biasedExp, mantissa) == true)
	{
		printf("ninf\n");
	}

	else
	{
	
		if (sign == -1 && mantissa == 0)
		{	printf("-"); }

		output[0] ='\0';
		if ( x < 0.0 )
		{
			append( output, '-' );
			x = -x;					// make x positive
		}
		get_float_digit( x, &c, &pow10, &remainder );
		append( output, c );
		append( output, '.' );
		x = remainder;
		plast = p10 = pow10;			// pow10 set by get_float_digit()
		for ( i = 1 ; i < 7 ; i++ )		// 7 significant digits in 32-bit float
		{
			get_float_digit( x, &c, &p10, &remainder );
			if ( (plast - p10) > 1 )
			{
				append( output, '0' );	// fill in zero to next nonzero digit
				plast -= 1;
			}
			else
			{
				append( output, c );
				x = remainder;
				plast = p10;
			}
		}
		if ( pow10 < 0 )		// negative exponent
		{
			exponent[0] = 'e';
			intToDecASCII( pow10, exponent+1 );
		}
		else if ( pow10 < 10 )		// positive single-digit exponent
		{
			exponent[0] = 'e';
			exponent[1] = '+';
			exponent[2] = '0';
			intToDecASCII( pow10, exponent+3 );
		}
		else				// positive multi-digit exponent
		{
			exponent[0] = 'e';
			exponent[1] = '+';
			intToDecASCII( pow10, exponent+2 );
		}
		strcat( output, exponent );
		printf("%s\n",output);
	}
}



void 
displayFloat(char * input)
{
	float		bits = getBitPattern(input);
	char *		output = createEmptyBitstring(10);

	floatToASCII(bits, output);
}








char * 
intToDecASCII(int pow, char * exp)
{
	int		start = 0;
	char *		backwardsNum;
	int		finalstringlen = 0;
	char		digit = '0';
	int		i = 0;
	int		powCopy;

	if (pow < 0)
	{
		exp[0] =  '-';
		pow *= -1;
		start++;
	}

	powCopy = pow;

	while (powCopy != 0)
	{
		finalstringlen++;
		powCopy /= 10;
	}

	
	backwardsNum = createEmptyBitstring(finalstringlen);
	finalstringlen = 0;
	

	if (pow > 0)
	{
		while (pow != 0 )
		{
			digit = (char)(0x30 + pow % 10);
			backwardsNum[finalstringlen] = digit;
			finalstringlen++;
			pow /= 10;

		}


		
		for (; i <= finalstringlen; i++, start++)
		{
			append(exp, backwardsNum[finalstringlen - i]); 
		} 

	}
	else { exp[start] = '0'; }

	

	return exp;

}


bool
isInvalid(int argc, char *argv[])
{
	int		i = 0;

	if (argc < 3)
	{
	       	printf("Too few arguments.\n");
		return true;
	}

	 if (argc > 3)
	{
	       	printf("Too many argumentsi.\n");
		return true;
	}

	
	if (strlen(argv[1]) > 32)
	{
		printf("You entered too many digits.\n");
		return true;
	}	

	if (strlen(argv[1]) < 32)
	{
		printf("You entered too few digits.\n");
		return true;
	}	


	if (strcmp(argv[2], intInput) != 0 && strcmp(argv[2], floatInput) != 0 )
	{
		printf("The requested output format is not supported.\n");
		return true;
	}

	for (; i < strlen(argv[1]); i++)
	{
		if (argv[1][i] != '0' && argv[1][i] != '1' )
		{
			printf("The character %c is displayed at position %d instead of binary\n",argv[1][i], i);
			return true;
		}
	}


	return false;
}


int 
main(int argc, char *argv[])
{
	/*Checking for valid inputs*/
	if (isInvalid(argc, argv))
	{
		return 0;
	}

	else
	{


		/*Relevant variables declared here*/
		char *	input = argv[1];
		char *	format = argv[2];

		/*Initialize values here*/



		/*Determine output format and print*/
		if (strcmp(format, intInput) == 0)
		{
			spitDecimal(input, 32, false);		
		}
	
		else
		{
			displayFloat(input);
		}

		return 0;
	}
}

