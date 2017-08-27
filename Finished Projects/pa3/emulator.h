#ifndef _EMULATOR_H_
#define _EMULATOR_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define eax 0
#define ecx 1
#define edx 2
#define ebx 3
#define esp 4
#define ebp 5
#define esi 6
#define edi 7

#define AOK "Everything is fine.\n"
#define INS "An invalid instruction.\n"
#define ADR "Some sort of invalid address.\n"
#define HLT "Halt instruction has been encountered.\n"

typedef union {

	unsigned char c;

	struct {
		unsigned int rB:4;
		unsigned int rA:4;
	} part;
} Byte;

typedef struct {
	
	int reg[8];
	int counter;
	unsigned int ZF:1;
	unsigned int OF:1;
	unsigned int SF:1;

} computer;

/*Main functions that facilitate the fetch-decode-execute loop*/
int creatememoryimage (char * filename);
int execute (Byte * program, computer * cpu);

/*Helper functions*/
bool invalidinput(int argc, char *argv[]);
void initialize(computer * cpu); //sets all registers and programs to 0
int hexliteral(char hex);
void getRegisters(Byte registers, int * first, int * second);
int fetchlong(Byte * program, computer * cpu, int offset);
int copyfrommemory(Byte * program, computer * cpu, int displacement, int reg);
void writetomemory (Byte * program, computer * cpu, int offset, int reg, int value);
void resetflags(computer * cpu);
void setflags(computer * cpu, char operation, int left, int right, int result);
char * regnumber(unsigned int reg);
#endif /* _EMULATOR_H_ */
