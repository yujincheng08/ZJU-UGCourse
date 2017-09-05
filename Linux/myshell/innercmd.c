#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/times.h>
#include <time.h>
#include "innercmd.h"
#include "prcsctrl.h"

//Define a marco to simplify defination of inner command funcion.
#define cmdFun(A) size_t (A)(Command *cmd, pid_t pgid, int pipefd[2])
//Define a type of cmdFun.
typedef cmdFun((*pcmdFun)); 

//Sturct to store an inner command.
typedef struct InnerCmd
{
	const char *cmd;	//Command name.
	pcmdFun fun;	//Pointer to the function.
}InnerCmd;

//Output current working directory.
cmdFun(pwd)
{
	//Fork a new process.
	pid_t pid;
	if((pid = fork())<0)
		return pid;
	else if(pid == 0)
	{
		//Set process group id as its pid.
		setpgid(pid,pgid);
		//Redirection accroding to cmd and pipefd.
		cmdDup(cmd,pipefd);
		//Get the working directory.
		//Function will automatically allocate memmory.
		char *pwd = getcwd(NULL,0);
		//Output it.
		puts(pwd);
		//Free it.
		free(pwd);
		//Remember to exit the process.
		exit(0);
	}
	else if(pid > 0)
	{
		//Set the child process group id as its pid.
		setpgid(pid,pgid);
		//Close the pipe in case of blocking the child process.
		closepipe(pipefd);
		//return the pid.
		return pid;
	}
	return 0;
}

cmdFun(cd)
{
	//If the second argument exists
	//and command does not appear in the meddle of its group.
	if(cmd->args[1]&&!(cmd->next))
	{
		//Change directory and update PWD.
		//But it will extend all symplic link.
		int result = chdir(cmd->args[1]);
		if(result == -1)
			perror("cd");
		char *PWD = getcwd(NULL,0);
		setenv("PWD",PWD,1);
		free(PWD);
		return result;
	}
	//Otherwise call the pwd function.
	else
		return pwd(cmd,pgid,pipefd);
}


//This two function simply exit the shell when there are no jobs
//running background.
cmdFun(myexit)
{
	if(jobRunning())
		fputs("There are still jobs running.\n",stderr);
	else
		exit(0);
	return -1;
}

cmdFun(quit)
{
	return myexit(cmd,pgid,pipefd);
}

//Clean the screen.
cmdFun(clr)
{
	//"\033[1A" means go to the line above.
	//"\033[2J" means clear the screen.
	//"\033[H" means go to the first line.
	static char clear[] = "\033[1A\033[2J\033[H";
	ssize_t result = write(STDOUT_FILENO,clear,strlen(clear));
	if(result==-1) return -1;
	else return 0;
}

cmdFun(myenviron)
{
	//Similiar partern.
	pid_t pid = fork();
	if(pid<0) return pid;
	else if(pid==0)
	{
		
		setpgid(pid,pgid);
		cmdDup(cmd,pipefd);
		//Declare extern value environ.
		extern char **environ; 
		char **env = environ;
		//Loop to output them all.
		while(*env)
			puts(*env++);
		exit(0);
	}
	else if(pid > 0)
	{
		setpgid(pid,pgid);
		closepipe(pipefd);
		return pid;
	}
}

cmdFun(mytime)
{
	//Fork a new process.
	pid_t pid = fork();
	if(pid < 0) return pid;
	if(pid == 0)
	{
		//Set the pgid as itself and update pgid value.
		setpgid(pid,pgid);
		pgid = getpid();
		//Check the argument.
		if(!cmd->args[1])
		{
			cmdDup(cmd,pipefd);
			char *strtime = (char *)malloc(sizeof(char)*20);
			memset(strtime, 0, sizeof(char)*20);
			time_t now;
			time(&now);
			strftime(strtime,20,"%Y-%m-%d %H:%M:%S",
						gmtime(&now));
			dprintf(STDOUT_FILENO,"Now time: %s\n",strtime);
			exit(0);
		}
		//Fork one more process to run command.
		pid = fork();
		if(pid < 0)
		{
			perror("time");
			exit(-1);
		}
		if(pid == 0)
		{
			//Now its pgid is its PPID.
			setpgid(pid,pgid);
			//Redirection for this process.
			cmdDup(cmd,pipefd);
			//Execute command.
			if(execvp(cmd->args[1],cmd->args+1)==-1)
				perror(cmd->args[1]);
			exit(-1);
		}
		else if(pid > 0)
		{
			struct tms stime,etime;
			double tck = (double)sysconf(_SC_CLK_TCK);
			setpgid(pid,pgid);
			closepipe(pipefd);
			//Start timing.
			clock_t real = times(&stime);
			if(waitpid(pid,NULL,0)==-1)
				perror(cmd->cmd);
			//End timing and get the real time.
			real = times(&etime) - real;
			//Out put the result.
			//Remember to use cutime and cstime.
			dprintf(STDOUT_FILENO,"\nreal\t%.3lfs\n",
					real/tck);
			dprintf(STDOUT_FILENO,"user\t%.3lfs\n",
					(etime.tms_cutime-stime.tms_cutime)/tck);
			dprintf(STDOUT_FILENO,"sys\t%.3lfs\n",
					(etime.tms_cstime-stime.tms_cstime)/tck);
			exit(0);
		}

	}
	else if(pid > 0)
	{
		setpgid(pid,pgid);
		closepipe(pipefd);
		return pid;
	}
}

cmdFun(dir)
{
	//Similiar partern.
	pid_t pid = fork();
	if(pid<0)return pid;
	else if(pid==0)
	{
		setpgid(pid,pgid);
		cmdDup(cmd,pipefd);
		DIR *dp;
		struct dirent *dirp;
		//Open the dir steam.
		if(cmd->args[1])
			dp = opendir(cmd->args[1]);
		else
			dp = opendir(getenv("PWD"));
		if(dp == NULL)
		{
			perror("dir");
			exit(-1);
		}
		//Loop to output every file and subdirectory.
		while((dirp = readdir(dp)) != NULL)
			dprintf(STDOUT_FILENO,"%s\n",dirp->d_name);
		//close the steam.
		closedir(dp);
		exit(0);
	}
	else if(pid>0)
	{
		setpgid(pid,pgid);
		closepipe(pipefd);
		return pid;
	}

}

cmdFun(echo)
{
	//Similiar partern.
	pid_t pid = fork();
	if(pid < 0) return pid;
	if(pid == 0)
	{
		
		setpgid(pid,pgid);
		cmdDup(cmd,pipefd);
		char * const *arg = cmd->args + 1;
		//Loop to print each argument.
		while(*arg)
		{
			dprintf(STDOUT_FILENO,"%s ",*arg);
			arg++;
		} 
		//And output a new line.
		dprintf(STDOUT_FILENO,"\n");
		exit(0);
	}
	else if(pid > 0)
	{
		setpgid(pid,pgid);
		closepipe(pipefd);
		return pid;
	}
}

cmdFun(help)
{
	//Similiar partern.
	pid_t pid = fork();
	if(pid < 0) return pid;
	if(pid == 0)
	{
		setpgid(pid,pgid);
		cmdDup(cmd,pipefd);
		//more readme
		if(execlp("more","more","readme",(char *)0)==-1)
			perror("help");
		exit(-1);
	}
	else if(pid > 0)
	{
		setpgid(pid,pgid);
		closepipe(pipefd);
		return pid;
	}
}

cmdFun(fg)
{
	//Simply froeground jobid.
	if(cmd->args[1])
		return foreground(atoi(cmd->args[1]));
	else return -1;
}

cmdFun(bg)
{
	//Simply background jobid.
	if(cmd->args[1])
		return background(atoi(cmd->args[1]));
	else return -1;
}

cmdFun(jobs)
{
	//Similiar pattern.
	pid_t pid = fork();
	if(pid<0) return pid;
	if(pid == 0)
	{
		setpgid(pid,pgid);
		cmdDup(cmd,pipefd);
		char * const *arg = cmd->args + 1;
		//If no arguments, print all the jobs.
		if(!*arg)
			printJob(0);
		//Otherwise print jobs of aguments id.
		else
			while(*arg)
				printJob(atoi(*arg++));
		exit(0);
	}
	else if(pid > 0)
	{
		setpgid(pid,pgid);
		closepipe(pipefd);
		return pid;
	}
}

//Simply shift the arguments.
cmdFun(shift)
{
	shiftarg();
	return 0;
}

cmdFun(test)
{
	//Similiar pattern.
	pid_t pid = fork();
	if(pid < 0) return pid;
	else if(pid == 0)
	{
		setpgid(pid,pgid);
		pgid = getpid();
		cmdDup(cmd,pipefd);
		//One more pocess.
		pid=fork();
		if(pid<0)
		{
			perror("test");
			exit(-1);
		}
		if(pid == 0)
		{

			setpgid(pid,pgid);
			//Execute the outer command test.
			if(execvp(cmd->cmd,cmd->args)==-1)
				perror(cmd->cmd);
			exit(-1);
		}
		else if(pid>0)
		{
			int statloc = 0;
			//Wait the process and print the result.
			if(waitpid(pid,&statloc,0)==-1)
				perror(cmd->cmd);
			if(statloc==0)
				puts("true");
			else
				puts("false");
			exit(0);
		}


	}
	else if(pid>0)
	{
		setpgid(pid,pgid);
		closepipe(pipefd);
		return pid;
	}

}

cmdFun(myumask)
{
	//If there is a arguments, simply call the umask.
	if(cmd->args[1])
	{
		umask(strtol(cmd->args[1],NULL,8));
	}
	else
	{
		//Well, we need to print the current umask.
		int outfileno = STDOUT_FILENO;
		//Firstly, set the output file discription.
		if(cmd->outfile&&cmd->outdict)
		{
			int flag = O_WRONLY | O_CREAT;
			if(cmd->outdict == 2)
				flag |= O_APPEND;
			mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
			outfileno = open(cmd->outfile,flag,mode);
			if(outfileno == -1)
			{
				perror(cmd->outfile);
				return -1;
			}
		}
		if(outfileno == STDOUT_FILENO && cmd->next)
			outfileno=pipefd[1];
		mode_t mode;
		//Set the umask to 0 and then set it back.
		//So we can get the current umask.
		umask((mode = umask(0)));
		//Print it with output file discription.
		dprintf(outfileno,"%04o\n",mode);
	}
	return 0;
}

cmdFun(set)
{
	//There have to be two arguments.
	if(!cmd->args[1]||!cmd->args[2])
	{
		dprintf(STDERR_FILENO,"set: Arguments error.\n");
		return -1;
	}
	//Simply set it.
	return setenv(cmd->args[1],cmd->args[2],1);
}

cmdFun(unset)
{
	//There has to be one arguments.
	if(!cmd->args[1])
	{
		dprintf(STDERR_FILENO,"set: Arguments error.\n");
		return -1;
	}
	//Simply unset it.
	return unsetenv(cmd->args[1]);
}

cmdFun(exec)
{
	//Simply redirect input and output, and then call exec.
	if(cmd->args[1])
	{
		cmdDup(cmd,pipefd);
		execvp(cmd->args[1],cmd->args + 1);
	}
	else
	{
		//It means we should run the shell itself.
		cmdDup(cmd,pipefd);
		execlp(getenv("myshell"),"myshell",(char *) 0);
	}
}

//All inner commands and their functions
static InnerCmd innerCmds []=
{
	{"cd",cd},
	{"pwd",pwd},
	{"time",mytime},
	{"clr",clr},
	{"dir",dir},
	{"environ",myenviron},
	{"echo",echo},
	{"help",help},
	{"quit",quit},
	{"exit",myexit},
	{"fg",fg},
	{"bg",bg},
	{"jobs",jobs},
	{"shift",shift},
	{"test",test},
	{"umask",myumask},
	{"set",set},
	{"unset",unset},
	{"exec",exec}
};

cid_t isInnercmd(const char *cmd)
{
	//Size of innerCmds array.
	size_t t = sizeof(innerCmds)/sizeof(InnerCmd);
	for(size_t i=0;i<t;i++)
	{
		//Is there and inner commands match the command?
		if(!strcmp(innerCmds[i].cmd,cmd))
			return i;
	}
	return -1;
}

pid_t execInnercmd(cid_t cid,Command *cmd, pid_t pgid, int pipefd[2])
{
	//Run the command function.
	return innerCmds[cid].fun(cmd, pgid, pipefd);
}

