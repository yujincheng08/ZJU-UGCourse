#include "cmdparser.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>

//Arguments count and arguments vector.
//Initialized by initEnv.
static int argc;
static char **argv;

//Struct to store a list of arguments.
typedef struct ArgList
{
	//Argument itself.
	char *arg;
	//Pointer to the next element.
	struct ArgList *next;
}ArgList;

//Function to read an string end with '\n' from stdin.
static char *readLine()
{
	//Buff and the size used by getline.
	//Declare as static for reusing.
	static char *buff = NULL;
	static size_t n = 0;
	//Store the length of the line.
	ssize_t len = 0;
	//Get a line.
	len = getline(&buff,&n,stdin);
	//If get EOF and buff gets nothing.
	if(len==-1&&feof(stdin))
	{
		if(isatty(STDIN_FILENO))
		{
			//Ignore it when stdin is a tty device.
			clearerr(stdin);
			return NULL;
		}
		else
		{
			//Exit the programe when stdin is not a tty device.
			//Of course output exit to prompt ending the program.
			//But not output when it's readed from a file.
			extern int RDFRMFILE;
			RDFRMFILE?:puts("exit");
			exit(0);
		}
	}
	//Unexpected situation.
	else if(len==-1||len==0)
		exit(-1);
	//Discard the '\n' charactor.
	buff[len-1]='\0';
	//Return it after dupping.
	return strdup(buff);
}

//Copy a string at range [p,q)
static char *strrcpy(char *dest, const char *p, const char * q)
{
	strncpy(dest,p,q-p+1);
	dest[q-p]='\0';
	return dest;
}

//Dup a string at range [p,q)
static char *strrdup(const char *p,const char *q)
{
	//Alloc memory for it.
	char *out = (char *)malloc((q-p+1)*sizeof(char));
	//Simply copy it.
	strncpy(out,p,q-p);
	out[q-p]='\0';
	return out;
}

//Function to judge weather c is an avaliable character, which is not a delimiter. 
static int avaliableChar(const char c)
{
	//' ', '&', '|' delimit two commands,
	//'<', '>' needs to redirect,
	//'\0' delimits two command groups.
	return (c!=' '&&c!='&'&&c!='|'&&c!='<'&&c!='>'&&c);
}

//Judge weather string is consist of digits.
static int isNum(const char *str)
{
	while(*str)
		if(!isdigit(*str++))
			return 0;
	return 1;
}

//Process arguments. Convert environment reference to its value.
static char *prcsArg(char *arg)
{
	//Static string to store the first argument,
	//which will not change after shifting.
	static char *arg0 = NULL;
	if(!arg0) arg0 = argv[0];
	//String starts with '$', which needs parsering.
	if(arg&&arg[0]=='$')
	{
		//If it's consist of digits,
		//try to convert arg to shell arguments.
		if(isNum(arg+1))
		{
			int num=atoi(arg+1);
			if(num==0)
			{
				//Free it and return a dup of arg0.
				free(arg);
				return strdup(arg0);
			}
			else if(num<argc&&num>0)
			{
				//Argument does exist,
				//free the old one and return the parsered one.
				free(arg);
				return strdup(argv[num]);
			}
			//Not exists.
			else return NULL;
		}
		//Directly return a dup form getenv.
		char *env = getenv(arg+1);
		if(env)
			return strdup(env);
		return NULL;
	}
	else
		//Well, no need to parse it.
		return arg;
}

//Get the file name of reading rediection from command string p.
static const char *readInRedirect(const char * p, Command *cmd)
{
	//Wrong call?
	if(*p!='<') return p;
	//Skip spaces.
	while(*++p==' ');
	//Floor pointer.
	const char *q = p;
	//Skip until meeting the first unavaliable character.
	while(avaliableChar(*++q));
	//Well, the file name is contain in [p,q). Dup it.
	cmd->infile = prcsArg(strrdup(p,q));
	return q;
}

static const char *readOutRedirect(const char *p, Command *cmd)
{
	//Wrong call?
	if(*p!='>') return p;
	//Which mode? Append or overwite?
	if(*(p+1)=='>') 
	{
		//Ok, append, set the flag.
		cmd->outdict=2;
		//Skip this character.
		p++;
	}
	//Ok, overwite.
	else cmd->outdict=1;
	//Similiar with readInRedirect().
	while(*++p==' ');
	const char *q = p;
	while(avaliableChar(*++q));
	cmd->outfile = prcsArg(strrdup(p,q));
	return q;
}

//Generate an empty Command object.
static Command *generateCmd()
{
	Command *cmd = (Command *)malloc(sizeof(Command));
	cmd->next = NULL;
	cmd->cmd = NULL;
	cmd->args = NULL;
	cmd->infile = cmd->outfile = NULL;
	cmd->outdict = 0;
	return cmd;
}

//Convert ArgList to an array vector.
static char** list2array(ArgList *list,int count)
{
	//Allocate one more pointer to mark the end with NULL.
	char **args = malloc(sizeof(char *)*(count+1));
	//Mark the end of the vector.
	args[count]=NULL;
	//Store the element to delete.
	ArgList *old;
	for(int i=0;i<count;i++)
	{
		//Copy it, skip it and free it.
		args[i]=list->arg;
		old = list;
		list = list->next;
		free(old);
	}
	//Vector done.
	return args;
}

void shiftarg()
{
	//Shift arguments. Simply decrese argc and increse argv.
	if(argc>0)
		argc--, argv++;
}

//Complete the given cmd with cmdstr.
//Return the starting index of the next command.
static const char *newCmd(const char *cmdstr, Command *cmd)
{
	//Two pointers.
	const char *p = cmdstr,*q;
	//Values to generate a list of arguments.
	ArgList *head=NULL,*pfore=NULL,*list;
	//Counter of arguments.
	int count;
	//After each loop, update p pointer to the latest position.
	for(count = 0;;p=q)
	{
		//Skip spaces.
		while(*p==' ')p++;
		if(*p=='|'||*p=='&'||!*p)
			//This indicates the end of current command.
			break;
		if(*p=='>')
		{
			//Redirection of output, parse when outfile not set.
			if(!(cmd->outfile))
				q=readOutRedirect(p,cmd);
			continue;
		}
		if(*p=='<')
		{
			//Similiar with above.
			if(!(cmd->infile))
				q=readInRedirect(p,cmd);
			continue;
		}
		//Set q and seek the end positon of the argument.
		q=p;
		while(avaliableChar(*++q));
		//Allocate new element.
		list = (ArgList *)malloc(sizeof(ArgList));
		list->next = NULL;
		//Dup [p,q) and parse it.
		list->arg = prcsArg(strrdup(p,q));
		//Well, empty one, skip it.
		if(!list->arg)
		{
			free(list);
			continue;
		}
		//Add it to the list.
		if(!head) head=list;
		if(pfore)
			pfore->next = list;
		pfore=list;
		//Update counter.
		count++;
	}
	//Convert args and set the first argument as cmd.
	cmd->args = list2array(head,count);
	cmd->cmd = cmd->args[0];
	return p;
}

//Complete the given group with cmdstr.
//Return the beginning position of the next group.
static const char *newCmdGroup(const char * cmdstr, CmdGroup *group)
{
	Command *head=NULL,*pfore=NULL,*list;
	const char *p = cmdstr;
	group->next=NULL;
	//Loop until meeting '&' or '\0' which indicate the end.
	for(p = cmdstr;*p&&*p!='&';)
	{
		//Generate a new element.
		list = generateCmd();
		//Command starts with '|' is normal, simply skip it.
		if(*p=='|') p++;
		//Complete the object.
		p = newCmd(p,list);
		//Add it to the list.
		if(!head) head = list;
		if(pfore) pfore->next = list;
		pfore = list;
	}
	group->cmd = head;
	//Set the endChar of the group.
	group->endChar = *p;
	//Set the raw command group string.
	while(*cmdstr==' ') cmdstr++;
	group->rawCmd = strrdup(cmdstr,p);
	return p;
}

//Read a line of command from stdin.
CmdGroup *readCmdLine()
{
	//Read a line of string.
	char *line = readLine();
	if(!line) return NULL;
	const char *p = line;
	CmdGroup *head=NULL,*pfore=NULL,*list;
	for(p = line;*p;)
	{
		//Allocate a new element.
		list = (CmdGroup *)malloc(sizeof(CmdGroup));
		//Group starts with '&' is normal, simply skip it.
		if(*p=='&') p++;
		//Complete it and add it to the list.
		p = newCmdGroup(p,list);
		if(!head) head = list;
		if(pfore) pfore->next = list;
		pfore = list;
	}
	extern int RDFRMFILE;
	if(!isatty(STDIN_FILENO)&&!RDFRMFILE)
		//If stdin is not a tty or a outer file, output prompt.
		puts(line);
	//Free line afer parsing.
	free(line);
	return head;
}

//Function to get the absolute path of myshell
//and set the myshell environment value.
static void initPath()
{
	char *path;
	//Size of buffer and the real length of the path.
	ssize_t buf=1024,len=0;
	//Link to read the absolute path.
	char link[]="/proc/self/exe";

	//Loop until get the whole path.
	while(1)
	{
		//Allocate memory and try to read the link.
		path = malloc(buf);
		len = readlink(link,path,buf);

		if(len < buf - 1)
			//Nice, get the whole path.
			break;
		//Size of the buffer is too small.
		//Multiply the buf size and try again.
		buf*=2;
		//Free the old one.
		free(path);
	}

	//Reallocate the size to free useless memory.
	path = realloc(path,len + 1);
	//Add the '\0' to the path.
	path[len]='\0';
	//Set environment value myshell.
	setenv("myshell",path,1);
	//Done. Free the path.
	free(path);
}

//Function to initialize the environement value.
void initEnv(int _argc, char *_argv[])
{
	//Initialize myshell.
	initPath();
	//Initialize argument values.
	if(_argc > 1)
		//At least two argument received.
		//Must read from an outer file.
		//$0 should be the name of the outer file not the shell.
		//Shift the argument manually.
		argc=_argc - 1, argv=_argv + 1;
	else
		//Well, Only one argument, no shifting.
		argc=_argc,	argv=_argv;
}

