#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


/*Uh, iterator never checks past this, so I think it's safe to make it this size*/
int num[200];

int
add(int one, int two)
{
	return one + two;
}


int dothething(int input)
{
	if (num[input] == -1) //this is the start state of the num array from main
	{
		if (input == 0)  //if at fibonacci(0)
		{
			num[input] = 0; //first seed value is 0
		}

		else if (input == 1)  //if at fibonacci(1)
		{
			num[input] = 1; //second seed value is 1
		}

		/*AHH STORING THE OLD VALUES IN THE ARRAY MAKES IT SO IT'S A LOT FASTER*/
		else
		{	num[input] = dothething(input - 2) + dothething(input - 1); }

	}

	return num[input];	//adds all fibonacci numbers in sequence up till input

}

int
main(int argc, char * argv[])
{
	int input = atoi(argv[1]);
	int i = 0; 	//iterator through loop


	for (; i < 200; i++) { num[i] = -1; }

	printf("Value:\t%d\n", dothething(input));	
	return 0;

}



