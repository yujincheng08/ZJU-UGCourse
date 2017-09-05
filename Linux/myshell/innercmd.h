#ifndef INNERCMD_H
#define INNERCMD_H
#include "cmdparser.h"
#include <sys/types.h>

//Define a new type: cid_t, meaning inner command id,
//which use to execute an inner command.
typedef ssize_t cid_t;
//Check if cmd refers to an inner command.
//If so, return cid_t for executing, otherwise return -1.
cid_t isInnercmd(const char *cmd);
//Execute an inner command.
//cid refers to a certain inner command.
//cmd prvides information of arguments, redirection and so on.
//pipfd is used for redirection.
//Return the pid of the child process if the inner command needs
//to fork a child process.
//Return 0 if unnesessary to fork.
//Return -1 if error occurs.
pid_t execInnercmd(cid_t cid,Command *cmd, pid_t pgid, int pipefd[2]);

#endif //INNERCMD_H
