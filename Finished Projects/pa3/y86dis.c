#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "emulator.h"


FILE *			output;
char *			outputname;
int			stop;

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
	

	outputname = malloc(sizeof(char) * strlen(filename) + 1);
	strcpy(outputname, filename);
	outputname[strlen(outputname) - 1] = 'm';
	outputname[strlen(outputname) - 2] = 's';
	outputname[strlen(outputname) - 3] = 'y';

	text = fopen(filename, "r");
	output = fopen(outputname, "w");

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
					fprintf(output,"%08x\t", address);
					fprintf(output, "%s\t", directive);
					/*byte chars can be thought of as longs*/
					fscanf(text, "%x", &longargument);
					program[address].c = (char)(longargument);
					fprintf(output ,"%02x\n", longargument);
				}
				

				else if (strcmp(directive, ".text") == 0)
				{
					cpu->counter = address;
					argument = malloc(1000);
					fscanf(text, "%s", argument);
					stop = ( (int) strlen(argument) ) / 2;

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
					fprintf(output,"%08x\t", address);
					fprintf(output, "%s\t", directive);
					//lets me use union to address memory individually
					//but also lets me overwrite 4 bytes at a time
					//first * talks about address pointed to
					//casts data like an int, and stores at 
					//the pointer + address as an offset
					*(int *)(program + address) = longargument;
					fprintf(output,"%08x\n", longargument);
				}
				

				else if (strcmp(directive, ".string") == 0)
				{
					getc(text); //moves past tab
					getc(text); //moves past first quote
					i = 1;

					fprintf(output,"%08x\t", address);
					fprintf(output, "%s\t", directive);
					fprintf(output, "\"");					

					while ((character = getc(text)) != '"')
					{
						*(unsigned char *)(program + address + i - 1) = (unsigned char)character;
						fprintf(output, "%c", character);
						i++;
					}

					fprintf(output, "\"\n");
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
	FILE * 			printingfile;
	char			character;
	int			label;

	stop += cpu->counter;	
	label = 0;

	fprintf(output, "\n\n\t.pos %x\n\n", cpu->counter);

	do
	{
		switch (program[cpu->counter].c)
		{
			/*nop*/
			case 0x00:

				fprintf(output,"%08x\tnop\n", cpu->counter);
				cpu->counter++;
				break;
			
			/*halt*/
			case 0x10:

				fprintf(output,"%08x\thalt\n", cpu->counter);
				cpu->counter++;
				break;

			/*rrmovl*/
			case 0x20:

				getRegisters(program[cpu->counter + 1], &first, &second);
				fprintf(output,"%08x\trrmovl\t\t%s, %s\n", cpu->counter, regnumber(first), regnumber(second));
			       	cpu->counter += 2;	
				break;

			/*irmovl*/
			case 0x30:

				getRegisters(program[cpu->counter + 1], &first, &second);
				cpu->reg[second] = fetchlong(program, cpu, 2);
				fprintf(output,"%08x\tirmovl\t\t$%d, %s\n", cpu->counter, cpu->reg[second], regnumber(second));
				cpu->counter += 6;
				break;

			/*rmmovl*/
			case 0x40:

				getRegisters(program[cpu->counter + 1], &first, &second);
				address = fetchlong(program, cpu, 2); //offset
				fprintf(output,"%08x\trmmovl\t\t%s, %d(%s)\n", cpu->counter, regnumber(first), address, regnumber(second));
				cpu->counter += 6;	
				break;	
			
			/*mrmovl*/
			case 0x50:
				
				getRegisters(program[cpu->counter + 1], &first, &second);
				address = fetchlong(program, cpu, 2); //offset
				fprintf(output,"%08x\tmrmovl\t\t%d(%s), %s\n", cpu->counter,address, regnumber(first), regnumber(second));
				cpu->counter += 6;
				break;

			/*addl*/
			case 0x60:

				getRegisters(program[cpu->counter + 1], &first, &second);
				fprintf(output,"%08x\taddl\t\t%s, %s\n", cpu->counter, regnumber(first), regnumber(second));
				cpu->counter += 2;		
				break;

			/*subl*/
			case 0x61:

				getRegisters(program[cpu->counter + 1], &first, &second);
				fprintf(output,"%08x\tsubl\t\t%s, %s\n", cpu->counter, regnumber(first), regnumber(second));
				cpu->counter += 2;		
				break;

			/*andl*/
			case 0x62:

				getRegisters(program[cpu->counter + 1], &first, &second);
				fprintf(output,"%08x\tandl\t\t%s, %s\n", cpu->counter, regnumber(first), regnumber(second));
				cpu->counter += 2;	
				break;

			/*xorl*/
			case 0x63:

				getRegisters(program[cpu->counter + 1], &first, &second);
				fprintf(output,"%08x\txorl\t\t%s, %s\n", cpu->counter, regnumber(first), regnumber(second));
				cpu->counter += 2;	
				break;

			/*mull*/
			case 0x64:
			
				getRegisters(program[cpu->counter + 1], &first, &second);
				fprintf(output,"%08x\tmull\t\t%s, %s\n", cpu->counter, regnumber(first), regnumber(second));
				cpu->counter += 2;	
				break;

			/*cmpl*/
			case 0x65:

				getRegisters(program[cpu->counter + 1], &first, &second);
				fprintf(output,"%08x\tcmpl\t\t%s, %s\n", cpu->counter, regnumber(first), regnumber(second));
				cpu->counter += 2;
				break;
			
			/*jmp*/
			case 0x70:

				address = fetchlong(program, cpu, 1);
				fprintf(output,"%08x\tjmp\t\t0x%08x\n", cpu->counter, address);
				cpu->counter += 5;
				break;

			/*jle*/
			case 0x71:

				address = fetchlong(program, cpu, 1);
				fprintf(output,"%08x\tjle\t\t0x%08x\n",cpu->counter, address);
				cpu->counter += 5;
				break;

			/*jl*/
			case 0x72:

				address = fetchlong(program, cpu, 1);
				fprintf(output,"%08x\tjl\t\t0x%08x\n", cpu->counter, address);
				cpu->counter += 5;
				break;

			/*je*/
			case 0x73:

				address = fetchlong(program, cpu, 1);
				fprintf(output,"%08x\tje\t\t0x%08x\n", cpu->counter, address);
				cpu->counter += 5;
				break;

			/*jne*/
			case 0x74:

				address = fetchlong(program, cpu, 1);
				fprintf(output,"%08x\tjne\t\t0x%08x\n", cpu->counter, address);
				cpu->counter += 5;
				break;
							
			/*jge*/
			case 0x75:

				address = fetchlong(program, cpu, 1);
				fprintf(output,"%08x\tjge\t\t0x%08x\n", cpu->counter, address);
				cpu->counter += 5;
				break;

			/*jg*/
			case 0x76:
				
				address = fetchlong(program, cpu, 1);
				fprintf(output,"%08x\tjg\t\t0x%08x\n", cpu->counter, address);
				cpu->counter += 5;
				break;

			/*call*/
			case 0x80:

				address = fetchlong(program, cpu, 1);
				fprintf(output,"%08x\tcall\t\t0x%08x\n", cpu->counter, address);
				cpu->counter += 5;
				break;

			/*ret*/
			case 0x90:
				fprintf(output,"%08x\tret\n", cpu->counter);

				cpu->counter += 1;
				break;

			/*pushl*/
			case 0xa0:

				getRegisters(program[cpu->counter + 1], &first, &second);
				if (first == 5)
				fprintf(output, "\n\t.L%i:\n", label++);
				fprintf(output,"%08x\tpushl\t\t%s\n", cpu->counter, regnumber(first));
				cpu->counter += 2;
				break;

			/*popl*/
			case 0xb0:

				getRegisters(program[cpu->counter + 1], &first, &second);
				fprintf(output,"%08x\tpopl\t\t%s\n", cpu->counter, regnumber(first));
				cpu->counter += 2;
				break;

			/*readb*/
			case 0xc0:

				getRegisters(program[cpu->counter + 1], &first, &second);
				address = fetchlong(program, cpu, 2);

				fprintf(output,"%08x\treadb\t\t%d(%s)\n", cpu->counter, address, regnumber(first));
				cpu->counter += 6;
				break;

			/*readl*/
			case 0xc1:

				getRegisters(program[cpu->counter + 1], &first, &second);
				fprintf(output,"%08x\treadl\t\t%d(%s)\n", cpu->counter, address, regnumber(first));
				address = fetchlong(program, cpu, 2);

				cpu->counter += 6;
				break;

			/*writeb*/
			case 0xd0:
				
				getRegisters(program[cpu->counter + 1], &first, &second);
				address = fetchlong(program, cpu, 2);
				fprintf(output,"%08x\twriteb\t\t%d(%s)\n", cpu->counter, address, regnumber(first));
				cpu->counter += 6;
				break;

			/*writel*/
			case 0xd1:

				getRegisters(program[cpu->counter + 1], &first, &second);
				address = fetchlong(program, cpu, 2);
				fprintf(output,"%08x\twritel\t\t%d(%s)\n", cpu->counter, address, regnumber(first));
				cpu->counter += 6;
				break;

			/*movsbl*/
			case 0xe0:

				getRegisters(program[cpu->counter + 1], &first, &second);
				address = fetchlong(program, cpu, 2);
				fprintf(output,"%08x\tmovsbl\t\t%d(%s), %s\n", cpu->counter, address, regnumber(first), regnumber(second));
				cpu->counter += 6;
				break;

			default:
				printf(INS);
				return 0;
				break;
		}

	} while (cpu->counter < stop);


	free(program);
	free(cpu);
	fclose(output);

	printingfile = fopen(outputname, "r");

	while ( (character = getc(printingfile)) != EOF)
		printf("%c", character);

	printf("\nWritten to file in %s\n", outputname);
	fclose(printingfile);
	free(outputname);
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
