#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "emulator.h"



bool
invalidinput(int argc, char *argv[])
{
	FILE *			test;

	if (argc < 2 || argc > 2)
		return true;
	if (strcmp(argv[1], "-h") == 0)
	{
		printf("This program executes Y86 code stored .y86 files.  Please execute this program again and provide a filename(within this directory) of the program you want to execute.\nFORMAT: ./y86emul prog1.y86\n");
		return true;
	}

	test = fopen(argv[1], "r");
	if (!test)
		return true;

	return false;
}

void
resetflags(computer * cpu)
{
	cpu->ZF = 0;
	cpu->OF = 0;
	cpu->SF = 0;
}


void 
setflags(computer * cpu, char operation, int left, int right, int result)
{
	resetflags(cpu);

	switch (operation)
	{
		case '+':
			if ( (left > 0 && right > 0 && result < 0) ||
					(left < 0 && right < 0 && result > 0) )
				cpu->OF = 1;
			break;
		case '-':
			//more complicated cases for left - right
			//+left - (- right) = - result, OVERFLOW
			//-left - (+ right) = + result, OVERFLOW
			if ( (left > 0 && right < 0 && result < 0) ||
					(left < 0 && right > 0 && result > 0) )
				cpu->OF = 1;
			break;

		case '*':
			//+left * -right = +result, OVERFLOW
			//-left * +right = +result, OVERFLOW
			//-left * -right = -result, OVERFLOW
			//+left * +right = -result, OVERFLOW
			if ( (left > 0 && right < 0 && result > 0) || 
					(left < 0 && right > 0 && result > 0) ||
					(left < 0 && right < 0 && result < 0) ||
					(left > 0 && right > 0 && result > 0) )
				cpu->OF = 1;
			break;

		//nothing to be done for overflow flags
		case '&':
		case '^':
			break;

		default:
			printf("Unregonized operation.\n");
			resetflags(cpu);
	}

	
	if (result == 0)
		cpu->ZF = 1;

	if (result < 0)
		cpu->SF = 1;
}


/*ASSUMPTION: cpu->size has a value*/
void 
initialize(computer * cpu)
{
	int	i = 0;

	resetflags(cpu);

	for (; i < 8; i++)
	{
		cpu->reg[i] = 0;
	}
	
}


int
hexliteral(char hex)
{
	/*CASE: numbers*/
	if (hex > 0x2F && hex < 0x3A)
		return hex - '0';
	/*CASE: a - f*/
	else if (hex > 0x60 && hex < 0x67)
		return 0x0a + (hex - 'a');
	/*CASE: A - F*/
	else if (hex > 0x40 && hex < 0x47)
		return 0x0a + (hex - 'A');

	printf(ADR);
	return 0;
}


void
getRegisters(Byte registers, int * first, int * second)
{
	*first = registers.part.rA;
	*second = registers.part.rB;
}

int
fetchlong(Byte * program, computer * cpu, int offset)
{
	return 	(int)( program[cpu->counter + offset].c +
			(program[cpu->counter + offset + 1].c << 8) +
			(program[cpu->counter + offset + 2].c << 16) +
			(program[cpu->counter + offset + 3].c << 24) );
}

int
copyfrommemory(Byte * program, computer * cpu, int displacement, int reg)
{
	return (int) ( (program[cpu->reg[reg] + displacement].c) +
	(program[cpu->reg[reg] + displacement + 1].c << 8) +
	(program[cpu->reg[reg] + displacement + 2].c << 16) +
	(program[cpu->reg[reg] + displacement + 3].c << 24) );
}

void
writetomemory(Byte * program, computer * cpu, int offset, int reg, int value)
{
	program[cpu->reg[reg] + offset].c = (value & 0x000000ff);
	program[cpu->reg[reg] + offset + 1].c = (value & 0x0000ff00) >> 8;
	program[cpu->reg[reg] + offset + 2].c = (value & 0x00ff0000) >> 16;
	program[cpu->reg[reg] + offset + 3].c = (value & 0xff000000) >> 24;
}


char* 
regnumber(unsigned int reg)
{
	switch (reg)
	{
		case 0:
			return "%eax";
			break;
		case 1:
			return "%ecx";
			break;
		case 2:
			return "%edx";
			break;
		case 3:
			return "%ebx";
			break;
		case 4:
			return "%esp";
			break;
		case 5:
			return "%ebp";
			break;
		case 6:
			return "%esi";
			break;
		case 7:
			return "%edi";
			break;
		default:
			return "unknown register";
			break;

	}

	return "unknown register";
}
