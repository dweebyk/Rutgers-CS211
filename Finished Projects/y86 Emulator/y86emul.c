#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "emulator.h"



/*fscanf(FILE, " %2x", &some int) to read stuff two ascii characters at a time*/
/*ASSUMPTIONS: there is one .size and it is the first directive*/
int 
creatememoryimage (char * filename)
{
	FILE *			text;
	computer *		cpu;
	int			size;
	int			i;	//used for iteration
	char *			directive;
	unsigned char		character;
	unsigned int		address; //all addresses are positive
	char *			argument; //used for .text and .string
	int			longargument; //used for .byte and .long
	Byte *			program;
	
	text = fopen(filename, "r");
	cpu = malloc(sizeof(computer));
	i = 0;

	if (text) //checks for success
	{
		while (!feof(text))
		{
			directive = malloc(sizeof(char)*10);
			fscanf(text, "%s", directive);

			if (strcmp(directive, ".size") == 0)
			{
				fscanf(text, "%x", &size);
				program = malloc(sizeof(Byte) * (size + 1));
			       	memset(program, '0', sizeof(Byte) * (size + 1));	
				cpu->counter = 0; //hasn't been assigned yet
				initialize(cpu);		
			}	

			else if (strcmp(directive, ".byte") == 0 || 
					strcmp(directive, ".long") == 0 || 
					strcmp(directive, ".string") == 0 ||
					strcmp (directive, ".text") == 0)
			{

				fscanf(text, "%x", &address);
			
				if (strcmp(directive, ".byte") == 0)
				{
					/*byte chars can be thought of as longs*/
					fscanf(text, "%x", &longargument);
					program[address].c = (char)(longargument);
				}
				

				else if (strcmp(directive, ".text") == 0)
				{
					cpu->counter = address;
					argument = (char *) malloc(1000);
					fscanf(text, "%s", argument);

					for (; i <= strlen(argument) - 2; i += 2)
					{	
						program[address + (i/2)].part.rA = hexliteral(argument[i]);
						program[address + (i/2)].part.rB = hexliteral(argument[i + 1]);
					}
					
					i = 0;
					free(argument);
				}
				
				else if (strcmp(directive, ".long") == 0)
				{
					fscanf(text, "%d", &longargument);
					//lets me use union to address memory individually
					//but also lets me overwrite 4 bytes at a time
					//first * talks about address pointed to
					//casts data like an int, and stores at 
					//the pointer + address as an offset
					*(int *)(program + address) = longargument;
				}
				

				else if (strcmp(directive, ".string") == 0)
				{
					getc(text); //moves past tab
					getc(text); //moves past first quote
					i = 1;

					while ((character = getc(text)) != '"')
					{
						*(unsigned char *)(program + address + i - 1) = (unsigned char)character;
						i++;
					}

					
					i = 0;
				}

				else
				{
					printf(INS);
				}
			
				free(directive);
			}
		}
	}
	else { printf("File doesn't exist.\n"); }

	fclose(text);

	return execute(program, cpu);
}



int
execute(Byte * program, computer * cpu)
{
	int 			first, second;
	int 			address;
	char 			character;
	int 			integer;
	int32_t 		castingint;
	bool 			hasNext;

	hasNext = true;	

	do
	{
		switch (program[cpu->counter].c)
		{
			/*nop*/
			case 0x00:

				cpu->counter++;
				break;
			
			/*halt*/
			case 0x10:

				printf(AOK);
				printf(HLT);
				hasNext = false;
				return 0;
				break;

			/*rrmovl*/
			case 0x20:

				getRegisters(program[cpu->counter + 1], &first, &second);
				cpu->reg[second] = cpu->reg[first];
			       	cpu->counter += 2;	
				break;

			/*irmovl*/
			case 0x30:

				getRegisters(program[cpu->counter + 1], &first, &second);
				cpu->reg[second] = fetchlong(program, cpu, 2);
				cpu->counter += 6;
				break;

			/*rmmovl*/
			case 0x40:

				getRegisters(program[cpu->counter + 1], &first, &second);
				address = fetchlong(program, cpu, 2); //offset
				writetomemory(program, cpu, address, second, cpu->reg[first]);
				cpu->counter += 6;	
				break;	
			
			/*mrmovl*/
			case 0x50:
				
				getRegisters(program[cpu->counter + 1], &first, &second);
				address = fetchlong(program, cpu, 2); //offset
				cpu->reg[first] = copyfrommemory(program, cpu, address, second);
				cpu->counter += 6;
				break;

			/*addl*/
			case 0x60:

				getRegisters(program[cpu->counter + 1], &first, &second);
				integer = cpu->reg[first] + cpu->reg[second];
				setflags(cpu, '+', cpu->reg[first], cpu->reg[second], integer);
				cpu->reg[second] = integer;
				cpu->counter += 2;		
				break;

			/*subl*/
			case 0x61:

				getRegisters(program[cpu->counter + 1], &first, &second);
				integer = cpu->reg[second] - cpu->reg[first];
				setflags(cpu, '-', cpu->reg[second], cpu->reg[first], integer);
				cpu->reg[second] = integer;
				cpu->counter += 2;		
				break;

			/*andl*/
			case 0x62:

				getRegisters(program[cpu->counter + 1], &first, &second);
				integer = cpu->reg[first] & cpu->reg[second]; //bitwise AND
				setflags(cpu, '&', cpu->reg[first], cpu->reg[second], integer);
				cpu->reg[second] = integer;
				cpu->counter += 2;	
				break;

			/*xorl*/
			case 0x63:

				getRegisters(program[cpu->counter + 1], &first, &second);
				integer = cpu->reg[first] ^ cpu->reg[second]; //bitwise OR
				setflags(cpu, '^', cpu->reg[first], cpu->reg[second], integer);
				cpu->reg[second] = integer;
				cpu->counter += 2;	
				break;

			/*mull*/
			case 0x64:
			
				getRegisters(program[cpu->counter + 1], &first, &second);
				integer = cpu->reg[first] * cpu->reg[second];
				setflags(cpu, '*', cpu->reg[first], cpu->reg[second], integer);
				cpu->reg[second] = integer;
				cpu->counter += 2;	
				break;

			/*cmpl*/
			case 0x65:

				getRegisters(program[cpu->counter + 1], &first, &second);
				integer = cpu->reg[second] - cpu->reg[first];
				setflags(cpu, '-', cpu->reg[second], cpu->reg[first], integer);
				cpu->counter += 2;
				break;
			
			/*jmp*/
			case 0x70:

				address = fetchlong(program, cpu, 1);
				cpu->counter = address;
				break;

			/*jle*/
			case 0x71:

				address = fetchlong(program, cpu, 1);
				/* (sf ^ of ) || zf*/
				if ( (cpu->SF == 1 && cpu->OF == 0) ||
						(cpu->SF == 0 && cpu->OF == 1) ||
						(cpu->ZF == 1) )
					cpu->counter = address;
				else
					cpu->counter += 5;
				break;

			/*jl*/
			case 0x72:

				address = fetchlong(program, cpu, 1);
				/* sf ^ ~of*/
				if ( (cpu->SF == 1 && cpu->OF == 0) ||
						(cpu->SF == 0 && cpu->OF == 1) )
					cpu->counter = address;
				else
					cpu->counter += 5;
				break;

			/*je*/
			case 0x73:

				address = fetchlong(program, cpu, 1);
				/* zf*/
				if (cpu->ZF == 1)
					cpu->counter = address;
				else
					cpu->counter += 5;
				break;

			/*jne*/
			case 0x74:

				address = fetchlong(program, cpu, 1);
				/* ~zf*/
				if (cpu->ZF == 0)
					cpu->counter = address;
				else
					cpu->counter += 5;
				break;
							
			/*jge*/
			case 0x75:

				address = fetchlong(program, cpu, 1);
				/* ~(sf ^ of)*/
				if ( !( (cpu->SF == 1 && cpu->OF == 0) || (cpu->SF == 0 && cpu->OF == 1) ) )
					cpu->counter = address;
				else
					cpu->counter += 5;
				break;

			/*jg*/
			case 0x76:
				
				address = fetchlong(program, cpu, 1);
				/* ~(sf ^ of) && ~zf*/
				if ( !( (cpu->SF == 1 && cpu->OF == 0) || (cpu->SF == 0 && cpu->OF == 1) ) &&
						cpu->ZF == 0)
					cpu->counter = address;
				else
					cpu->counter += 5;
				break;

			/*call*/
			case 0x80:

				address = fetchlong(program, cpu, 1);
				cpu->reg[esp] -= 4;
				integer = cpu->counter + 5; //store current address for ret
				writetomemory(program, cpu, 0, esp, integer);
				cpu->counter = address;
				break;

			/*ret*/
			case 0x90:

				cpu->counter = copyfrommemory(program, cpu, 0, esp);
				cpu->reg[esp] += 4;
				break;

			/*pushl*/
			case 0xa0:

				getRegisters(program[cpu->counter + 1], &first, &second);
				cpu->reg[esp] -= 4;
				writetomemory(program, cpu, 0, esp, cpu->reg[first]);
				cpu->counter += 2;
				break;

			/*popl*/
			case 0xb0:

				getRegisters(program[cpu->counter + 1], &first, &second);
				cpu->reg[first] = copyfrommemory(program, cpu, 0, esp);
				cpu->reg[esp] += 4;
				cpu->counter += 2;
				break;

			/*readb*/
			case 0xc0:

				getRegisters(program[cpu->counter + 1], &first, &second);
				address = fetchlong(program, cpu, 2);

				resetflags(cpu);

				if (EOF == scanf("%c", &character))
					cpu->ZF = 1;
				else
					cpu->ZF = 0;
				
				program[cpu->reg[first] + address].c = character;
				cpu->counter += 6;
				break;

			/*readl*/
			case 0xc1:

				getRegisters(program[cpu->counter + 1], &first, &second);
				address = fetchlong(program, cpu, 2);

				resetflags(cpu);

				if (EOF == scanf("%i", &integer) )
					cpu->ZF = 1;
				else
					cpu->ZF = 0;
				
				writetomemory(program, cpu, 0, first, integer);
				cpu->counter += 6;
				break;

			/*writeb*/
			case 0xd0:
				
				getRegisters(program[cpu->counter + 1], &first, &second);
				address = fetchlong(program, cpu, 2);
				printf("%c", program[cpu->reg[first] + address].c);
				cpu->counter += 6;
				break;

			/*writel*/
			case 0xd1:

				getRegisters(program[cpu->counter + 1], &first, &second);
				address = fetchlong(program, cpu, 2);
				integer = (int)copyfrommemory(program, cpu, address, first);
				printf("%i", integer);
				cpu->counter += 6;
				break;

			/*movsbl*/
			case 0xe0:

				getRegisters(program[cpu->counter + 1], &first, &second);
				address = fetchlong(program, cpu, 2);
				character = program[cpu->reg[second] + address].c;
				castingint = (int32_t) character;
				cpu->reg[first] = castingint;
				cpu->counter += 6;
				break;

			default:
				printf(INS);
				return 0;
				break;
		}

	} while (hasNext);


	free(program);
	free(cpu);
	return 0;
}


int
main (int argc, char * argv[])
{

	if (!invalidinput(argc, argv))
	{
		//...i realized late that I had both program and cpu made in image
		//so I just changed it to execute within the method
		return creatememoryimage(argv[1]);
	}
	return 0;
}
