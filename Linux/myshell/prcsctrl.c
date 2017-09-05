#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include "prcsctrl.h"
#include "innercmd.h"

//Struct to contain a list of jobs.
typedef struct Job
{
	pid_t pgid;	//Jobs' process group id.
	pid_t prcsCount;	//Number of job process.
	pid_t jobId;	//The id of the job.
	const char *cmd;	//Raw command of the jobs.
	struct Job *next;	//Pointer to the next job.
}Job;

//jobs is the first element of the job list.
//foreJob is the job running foreground.
static Job *jobs = NULL,*foreJob = NULL;

//Find the job before the given job.
static Job *findLast(Job *job)
{
	Job *last = jobs;
	while(last)
	{
		//Found. Return it.
		if(last->next == job)
			return last;
		//Skip it.
		last = last->next;
	}
	//Not found, return NULL.
	return NULL;
}

//Find a job with jobId.
static Job *findJob(pid_t jobId)
{
	Job *job = jobs;
	while(job)
	{
		//Found. Return it.
		if(job->jobId == jobId)
			return job;
		//Skip it.
		job=job->next;
	}
	return NULL;
}

void printJob(pid_t jobId)
{
	Job *job;
	//jobId==0 means print all.
	if(jobId==0)
	{
		job=jobs;
		while(job)
		{
			dprintf(STDOUT_FILENO,"[%d]%d\t\t%s\n",
						job->jobId,job->pgid,job->cmd);
			job=job->next;
		}
	}
	//Otherwise print a certain one.
	else
	{
		job=findJob(jobId);
		if(job)
		  dprintf(STDOUT_FILENO,"[%d]%d\t\t%s\n",
					job->jobId,job->pgid,job->cmd);
		else
			fputs("Error job id.\n",stderr);
	}
}

//Add the job to the list.
static void addJob(Job *job)
{
	Job *last = jobs;
	//Find avaliable job id.
	pid_t jobId = 0;
	//Find the greatest job id.
	while(last)
	{
		if(jobId < last->jobId)
			jobId = last->jobId;
		last = last->next;
	}
	//Then got the avaliable job id.
	job->jobId = jobId + 1;
	//Find the last job.
	last = findLast(NULL);
	//Well, the list is empty. Set job as the head element.
	if(!last)
		jobs = job;
	//Otherwise, add to the end of the list.
	else
		last->next = job;
	//Remember to seal the list.
	job->next = NULL;
	//Print it.
	printJob(job->jobId);
}

//Call this function when the job is done.
static void jobDone(Job *job)
{
	//Delete the element from the job list.
	Job *last = findLast(job);
	if(!last)
		jobs=job->next;
	else
		last->next = job->next;
	//Print prompt.
	dprintf(STDOUT_FILENO,"Job[%d] Done:\t\t%s\n"
				,job->jobId,job->cmd);
	//Free the cmd and the job.
	free((void*)(job->cmd));
	free(job);
}

//Call this function when a process of a job done.
static void prcsDone(Job *job)
{
	//Decrease the counter.
	job->prcsCount--;
	//All process done, the job also done.
	if(job->prcsCount == 0)
		jobDone(job);
}

//Function to wait the foreground job done.
static void waitForeground()
{
	//Set the terminal controler as the job.
	tcsetpgrp(STDIN_FILENO,foreJob->pgid);
	//If foreJob exists and its process counter is not zero.
	while(foreJob&&foreJob->prcsCount>0)
	{
		//Wait for it, but nohanging.
		//When blocking, we cannot stop this loop manually.
		if(waitpid(-foreJob->pgid,NULL,WNOHANG)>0)
			foreJob->prcsCount--;
	}
	//Foregound job ended, set the tc.
	tcsetpgrp(STDIN_FILENO,getpgrp());
}

int jobRunning()
{
	//Job list is not empty.
	if(jobs)
		return 1;
	else
		return 0;
}

//put a job foreground.
int foreground(pid_t jobId)
{
	//Find the job.
	Job *job = findJob(jobId);
	if(!job) 
	{
		fputs("Job not found.\n",stderr);
		return -1;
	}
	//Set the tc as the job.
	tcsetpgrp(STDIN_FILENO,job->pgid);
	if(kill(-job->pgid,SIGCONT)<0)
	{
		fputs("Error continue.\n",stderr);
		return -1;
	}
	//Set the foreJob and wait the foreground.
	foreJob = job;
	waitForeground();
	return 0;
}

//Call it when the foreground job stopped.
static void foreStop()
{
	if(!foreJob) return;
	//Declare this two static values.
	//For counting the process number.
	//If using pipe, system will automatically
	//stop the blocking process.
	//So use this two value to count the process.
	static Job *lastjob = NULL;
	static pid_t jobcount = 0;
	//New foreJob set, we need to update the two values..
	if(!lastjob||lastjob!=foreJob)
	{
		lastjob = foreJob;
		jobcount = 0;
	}
	//If some processes of the foreground job did stop,
	//update the counter.
	//Only all the process of the job stopped,
	//we can say that the job is set to background.
	//Or we can say that ctrl+z is pressed.
	if(waitid(P_PGID,foreJob->pgid,NULL,WNOHANG|WSTOPPED)==0
				&&++jobcount == foreJob->prcsCount)
	{
		//Update the two values.
		jobcount = 0;
		lastjob = NULL;
		Job *job = foreJob;
		//Set the foreground job to NULL,
		//which will end the loop in waitForeground().
		foreJob = NULL;
		//Print the information.
		dprintf(STDOUT_FILENO,"%c",'\n');
		//If the job is not in the job list, add it.
		if(job->jobId==0)
			addJob(job);
		//Set the tc as the shell.
		tcsetpgrp(STDIN_FILENO,getpgrp());
	}
}

//Iterate all groups to wait their ended process.
//One process at a time.
//Return the pid of the ended process.
//If no process ended, return -1.
static pid_t waitGroup()
{
	Job *job = jobs;
	pid_t pid = 0;
	while(job)
	{
		pid = waitpid(-jobs->pgid,NULL,WNOHANG);
		if(pid > 0)
		{
			//A process of the job done.
			prcsDone(job);
			return pid;
		}
		//Next job.
		job = job->next;
	}
	return -1;

}

//The handler of SIGCHLD.
static void sigchldhdlr(int sig)
{
	//Check the foreground.
	foreStop();
	//Loop until no process ended.
	while(waitGroup()>0);
}

//Redirect the input and output for the current process.
//Accroding to cmd information and pipe.
int cmdDup(Command *cmd, int pipefd[2])
{
	//By default, use stdin and stdout.
	int infileno = STDIN_FILENO;
	int outfileno = STDOUT_FILENO;
	//If the infile set, redirect stdin to it.
	if(cmd->infile)
	{
		//Try to open the file.
		infileno = open(cmd->infile,O_RDONLY);
		if(infileno!=-1)
			//Dup it.
			dup2(infileno,STDIN_FILENO);
		else
		{
			//Error occurs.
			perror(cmd->infile);
			return -1;
		}
	}
	//If outfile and outdict set, redirect stdout.
	if(cmd->outfile&&cmd->outdict)
	{
		//Write only and create the file.
		int flag = O_WRONLY | O_CREAT;
		//It has to append the file.
		if(cmd->outdict == 2)
			flag |= O_APPEND;
		//Set the mode. It equals to 0611.
		mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
		//Open it.
		outfileno = open(cmd->outfile,flag,mode);
		if(outfileno != -1)
			//Dup it.
			dup2(outfileno,STDOUT_FILENO);
		else
		{
			//Error occurs.
			perror(cmd->outfile);
			return -1;
		}
	}
	//Redirect to the pipe.
	//But only when no direction set above.
	if(infileno == STDIN_FILENO)
		dup2(pipefd[0],STDIN_FILENO);
	if(outfileno == STDOUT_FILENO && cmd->next)
		dup2(pipefd[1],STDOUT_FILENO);
}

//Simply send SIGCONT to the job group.
int background(pid_t jobId)
{
	Job *job = findJob(jobId);
	if(job)
		return kill(-job->pgid,SIGCONT);
	fputs("Error job id.\n",stderr);
	return -1;
}

//The handler of SIGINT.
static void siginthdlr(int signo)
{
	//Simply send the SIGINT to foreground job.
	if(foreJob)
		kill(-foreJob->pgid,SIGINT);
}

//The handler of SIGTSTP.
static void sigtstphdlr(int signo)
{
	//Ingore it.
	return;
}

void initSig()
{
	signal(SIGCHLD,sigchldhdlr);
	signal(SIGINT,siginthdlr);
	signal(SIGTSTP, sigtstphdlr);
	signal(SIGQUIT, SIG_IGN);

	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
}

//Close the given pipe for the main process in case of blocking.
void closepipe(int pipefd[2])
{
	//close them when they are not stdin or stdout.
	if(pipefd[0]!=STDIN_FILENO)
		close(pipefd[0]);
	if(pipefd[1]!=STDOUT_FILENO)
		close(pipefd[1]);
}

//Execute a command.
//Return the pid of the pgid of the child process group.
static pid_t cmdExec(Command *cmd, pid_t pgid, int pipefd[2])
{
	//Flush the stdin and stdout in case of interference.
	fflush(stdin);fflush(stdout);//Important.
	//Check weather it is an inner command.
	cid_t cid = isInnercmd(cmd->cmd);
	if(cid != -1)
		//Is a inner command, execute it.
		return execInnercmd(cid,cmd,pgid,pipefd); 
	//Fork a new child process.
	pid_t pid;
	if((pid = fork())<0)
		return pid;
	else if(pid == 0)
	{
		//Set the pgid.
		setpgid(pid,pgid);
		setenv("parent",getenv("myshell"),1);
		unsetenv("myshell");
		if(cmdDup(cmd,pipefd)==-1)
			exit(-1);
		//Execute the command.
		if(execvp(cmd->cmd,cmd->args)==-1)
			perror(cmd->cmd);
		exit(-1);
	}
	else if(pid > 0)
	{
		//Set the pgid for the child process.
		setpgid(pid,pgid);
		//Close the pipe.
		closepipe(pipefd);
	}
	//Free the arguments vector.
	char * const *arg = cmd->args;
	while(*arg)
		free((void*)*arg++);
	free((void*)cmd->args);
	//Return the pid.
	return pid;
}
//Create a empty Job object.
static Job *newJob()
{
	Job *job = (Job *)malloc(sizeof(Job));
	job->pgid = 0, job->prcsCount = 0;
	job->next = NULL;
	job->jobId = 0;
	return job;
}

//Execute a group of commands.
static int groupExec(CmdGroup *group)
{
	Command *cmd = group->cmd;
	//pipefd used for pipe().
	//fd used for the child process to dup.
	int pipefd[2]={STDIN_FILENO,STDOUT_FILENO},fd[2],closefd;
	//Store the Command object to free.
	Command *old = NULL;
	//New a job and set the rawCmd.
	Job *job = newJob();
	job->cmd = group->rawCmd;
	while(cmd)
	{
		//Input from the pipe.
		fd[0] = pipefd[0];
		//It has the next cmd object.
		//We should make a pipe between them.
		if(cmd->next)
		{
			if(pipe(pipefd) == -1)
				perror("pipe");
		}
		//Otherwise, output to stdout.
		else
			pipefd[1] = STDOUT_FILENO;
		//Output to the pipe.
		fd[1] = pipefd[1];
		//Execte the command.
		pid_t pid = cmdExec(cmd,job->pgid,fd);
		//Error occurs.
		if(pid < 0) return -1;
		//Set the pgid.
		if(!job->pgid) job->pgid = pid;
		//There is a new child process. Increase the counter.
		if(pid > 0)
			job->prcsCount++;
		//Jump to the next command and free this command object.
		old = cmd;
		cmd = cmd->next;
		free(old);
	}
	//Well, the job should run backgroud.
	if(group->endChar == '&')
		addJob(job);
	//Well, it should run foreground.
	else
	{
		//Set the foreJob and simply wait it.
		//No need to add to the job list.
		//It is just a temporary job.
		foreJob = job;
		waitForeground();
		//After waiting, free the foreJob.
		//But it may have set to the background.
		//Check it first.
		if(foreJob == job)
		{
			free((void*)(foreJob->cmd));
			free(foreJob);
			foreJob=NULL;
		}
	}
	return 0;
}

//Execute all the command groups in the same list.
int groupsExec(CmdGroup *group)
{
	//Initialize the signal first.
	initSig();
	CmdGroup *old;
	//Iterate the gorup list.
	int flag;
	while(group)
	{
		//Execute one group.
		if(groupExec(group)==-1&&flag==0)
			//Error occurs.
			flag=-1;
		//Jump to the next group and free the current group.
		old = group;
		group = group->next;
		free(old);
	}
	//Successed
	return flag;
}

