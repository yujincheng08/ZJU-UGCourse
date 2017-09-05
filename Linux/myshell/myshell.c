#include "myshell.h"

//Flag to mark weather lunch with outer file.
int RDFRMFILE = 0;

//Function to redirect input to an existing file.
int readfile(const char* path)
{
	//Set the flag and open it.
	RDFRMFILE=1;
	int fd = open(path,O_RDONLY);
	if(fd == -1)
		return -1;
	//Successfully open it, dup it.
	return dup2(fd,STDIN_FILENO);
}

int main(int argc,char *argv[])
{
	if(argc>1)
	{
		//Need to read from outer file.
		if(readfile(argv[1])==-1)
		{
			perror("myshell");
			exit(1);
		}
	}
	//Two function to initialize the shell.
	initEnv(argc,argv);
	initSig();
	//Command group to read and execute.
	CmdGroup *group = NULL;
	while(1)
	{
		if(argc==1)
			//Print the prompt string.
			printf("myshell:%s> ",getenv("PWD"));
		//Read command group from stdin.
		group = readCmdLine();
		if(group)
			//Successfully readed, execute it.
			groupsExec(group);
	}
	return 0;
}


