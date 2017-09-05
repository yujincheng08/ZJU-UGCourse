#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/wait.h>

//Structure to contain message.
struct msgmbuf
{
	long msgType;
	char msgText[512];
};

int main()
{
	//Generate keys and qids.
	key_t key12=ftok(".",0),key21=ftok(".",1);
	int qid12 = msgget(key12,IPC_CREAT|0666),qid21=msgget(key21,IPC_CREAT|0666);
	//Flush stdio and fork a new child process.
	fflush(stdin),fflush(stdout);
	pid_t pid = fork();
	if(pid>0)
	{
		//Fork one more new child process.
		pid=fork();
		if(pid==0)
		{
			//Print current process's information.
			printf("I am child process p1 and my PID is: %d.\n",getpid());
			//Generate message and send it.
			struct msgmbuf msg={getpid(),"Child process p1 is sending a message!"};
			size_t len = strlen(msg.msgText);
			msgsnd(qid12,&msg,len,0);
			//Wait for receiving message.
			msgrcv(qid21,&msg,512,0,0);
			//Print the message.
			printf("p1 recieved: %s\n",msg.msgText);
			exit(0);
		}
		else
		{
			//Main process.
			printf("I am main process and my PID is: %d.\n",getpid());
			//Wait two child processes.
			wait(NULL);
			wait(NULL);
			exit(0);
		}
	}
	else
	{
		printf("I am child process p2 and my PID is: %d.\n",getpid());
		//One more new process.
		fflush(stdin),fflush(stdout);
		pid=fork();
		if(pid==0)
		{
			printf("I am child process p3 and my PID is: %d.\n",getpid());
			if(execlp("ls","ls","-lA","--color=auto",NULL)==-1)
			{
				perror("exec");
				exit(-1);
			}
		}
		//Wait for child process.
		wait(NULL);
		//Similiar as p1.
		struct msgmbuf msg={getpid(),"Child process p2 is sending a message!"};
		size_t len = strlen(msg.msgText);
		msgsnd(qid21,&msg,len,0);
		msgrcv(qid12,&msg,512,0,0);
		printf("p2 recieved: %s\n",msg.msgText);
		exit(0);
	}
	return 0;
}

