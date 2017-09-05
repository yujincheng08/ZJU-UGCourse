#ifndef CMDPARSER_H
#define CMDPARSER_H

//Struct to store a list of commands.
typedef struct Command
{
	char *cmd;
	char * const *args;
	char *infile; //NULL means no, otherwise means a input file.
	int outdict; //0 means no, 1 means overwirte, 2 means append.
	char *outfile;	//File path for output redirect.
	struct Command *next;
	//Pointer to the next command in the smae group.
	//If exists, they are connected with pipe.
}Command;

//Struct to store a list of command gourps.
//Commands are in the same group when connected with pipe '|'.
//Commands groups are divided by '&'.
typedef struct CmdGroup
{
	Command *cmd;	//The first command of the group.
	char endChar;	//'&' or '\0', which mean foreground or background.
	const char *rawCmd;	//Store the raw command.
	struct CmdGroup *next;	//Pointer to the next group.
}CmdGroup;

//Function to read a line from stdin
CmdGroup *readCmdLine();

//Function to initialize static value argc and argv which are used to parse environment value.
void initEnv(int argc, char *argv[]);

//Function to shift arguments.
void shiftarg();

#endif //CMDPARSER_H
