#include <stdio.h>
#include "main.h"
#include "compute.h"
#include "input.h"

main()
{
	double x,y;
	printf("This program gets the values of x and y "
			"and output the reuslt of y powers of x.\n");
	x = input(PROMPT1);
	y = input(PROMPT2);
	printf("y powers of x is: %6.3f\n", compute(x,y));
}

