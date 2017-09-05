#ifndef PRCSCTRL_H
#define PRCSCTRL_H
#include "cmdparser.h"
#include <sys/types.h>

//Execute groups of command.
int groupsExec(CmdGroup *group);
//Redirect input and output according to the information
//of Command object and pipefd.
int cmdDup(Command *cmd, int pipefd[2]);
//Print the information of job with jodId.
//Or print all the jobs when jobId==0.
void printJob(pid_t jobId);
//Put job with jobId froegound.
int foreground(pid_t jobId);
//Continue job with jobId background.
int background(pid_t jobId);
//Close pipe for the main process in case of blocking.
void closepipe(int pipefd[2]);
//Initialize signals.
void initSig();
//Return if there is any jobs running background.
int jobRunning();

#endif //PRCSCTRL_H
