#include <stdio.h>
#include <pthread.h>

//Array to store the result.
//The biggest ull can content is the 89th Fibonacci sequence.
unsigned long long fib[90];

//Function for a new thread to complete the array.
void *generate(void *n);

int main()
{
	//Hints for user.
	printf("Input the length of the sequence you wanna generate (not greater than 90): ");
	//The number of the elements to generate.
	int n;
	scanf("%d",&n);
	if(n>90)
	{
		//Out of capacity.
		fprintf(stderr,"Error: An integer greater than 90 recieved!\n");
		n=90;
	}
	//A new thread.
	pthread_t thread;
	if(pthread_create(&thread,NULL,&generate,&n)==-1)
	{
		fprintf(stderr,"Error occured when creating new pthread!\n");
		return -1;
	}
	//Wait for the thread.
	pthread_join(thread,NULL);

	//Loop to print all the elements.
	for(int i=0,j=1;i<n;i++,j++)
	{
		printf("%-20llu",fib[i]);
		if(!(j%5)) putchar('\n');
	}
	//Print a new line.
	if(n%5)	printf("\n");
	return 0;
}

void *generate(void *n)
{
	int m = *(int*)n;
	//Initial condition.
	fib[0]=0,fib[1]=1;
	for(int i=2;i<m;i++)
	{
		//Generate.
		fib[i]=fib[i-1]+fib[i-2];
	}
}

