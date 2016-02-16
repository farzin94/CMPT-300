#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "tokenizer.h"
#define COMMAND_LENGTH 1024
#define NUM_TOKENS (COMMAND_LENGTH / 2 + 1)

/**
* Read a command from the keyboard into the buffer 'buff' and tokenize it
* such that 'tokens[i]' points into 'buff' to the i'th token in the command.
* buff: Buffer allocated by the calling code. Must be at least
* COMMAND_LENGTH bytes long.
* tokens[]: Array of character pointers which point into 'buff'. Must be at
* least NUM_TOKENS long. Will strip out up to one final '&' token.
* 'tokens' will be NULL terminated.
* in_background: pointer to a boolean variable. Set to true if user entered
* an & as their last token; otherwise set to false.
*/
char tokenize_command(char *buff, char *tokens[])
{
	int n = 0;
	for(char * tokenizer = strtok(buff, " "); tokenizer; tokenizer = strtok(NULL, " "))
	{
		printf(".\n");
		tokens[n] = tokenizer;
		n++;
		printf ("%s\n",tokenizer);
		printf(",\n");
	}
	for (int i = 1; i < n; i++)
	{	
		tokens[i][-1] = '\0';
	}
	
	tokens[n]=NULL;
}
void read_command(char *buff, char *tokens[], _Bool *in_background)
{
	*in_background = false;

	// Read input
	
	int length = read(STDIN_FILENO, buff, COMMAND_LENGTH-1);
	if ( (length < 0) && (errno !=EINTR) )
	{
		perror("Unable to read command. Terminating.\n");
		exit(-1); /* terminate with error */
	}

	// Null terminate and strip \n.
	buff[length] = '\0';
	if (buff[strlen(buff) - 1] == '\n') {
		buff[strlen(buff) - 1] = '\0';
	}


	// Tokenize (saving original command string)
	int token_count = tokenize_command(buff, tokens);
	if (token_count == 0) {
		return;
	}

	// Extract if running in background:
	if (token_count > 0 && strcmp(tokens[token_count - 1], "&") == 0) {
		*in_background = true;
		tokens[token_count - 1] = 0;
	}
}

/**
* Main and Execute Commands
*/
int main(int argc, char* argv[])
{
	char input_buffer[COMMAND_LENGTH];
	char *tokens[NUM_TOKENS];
	int status;
	while (true) {
	int result;
	int ret;
	char *currentDir = (getcwd(NULL, 0));
	char *tmpDir;

// Get command
// Use write because we need to use read()/write() to work with
// signals, and they are incompatible with printf().
	write(STDOUT_FILENO, "> ", strlen("> "));
	_Bool in_background = false;
	read_command(input_buffer, tokens, &in_background);

	if(tokens[0] != NULL)
	{
		if (strcmp(tokens[0], "cd") == 0)
		{
			if (strchr(tokens[1], '/'))
			{
        		ret = chdir(tokens[1]);
        		printf( "Directory changed to %s\n", tokens[1]);
        	}
        	else
        	{
        		tmpDir = strcat(currentDir, "/");
        		tokens[1] = strcat(tmpDir, tokens[1]);
        		ret = chdir(tokens[1]);
        		printf("The new token1 is %s\n", tokens[1]);
        	}
    	}
    	else if (strcmp(tokens[0], "cd") != 0)
   	 	{
        	perror( tokens[1] );
    	}
    }
/**
* Steps For Basic Shell:
* 1. Fork a child process
* 2. Child process invokes execvp() using results in token array.
* 3. If in_background is false, parent waits for
* child to finish. Otherwise, parent loops back to
* read_command() again immediately.
*/
pid_t tokPid = fork();
	
	if (tokPid == -1)
	{
		printf("broken");
	}
	
	if (tokPid == 0)
	{
		printf("Child: I'm the Child: %d\n", tokPid);
		printf("FOOBAR%s %s\n", tokens[0], tokens[1]);
		//execvp(tokens[0], NULL);
		result = execvp(tokens[0], tokens);
		printf("execvp %d execvp\n\n", result);
		printf("Oh dear, something went wrong with read()! %s\n", strerror(errno));
    }
    
    if (tokPid > 0)
	{
		if (in_background == false)
		{
    		waitpid(tokPid, &result, 0);
    	}
    	else 
    	{
        	continue;
    	}
		printf("Parent: I'm the Parent: %d\n", tokPid);
		while (waitpid(-1, NULL, WNOHANG) > 0) 
		{
		}
	}
	}
	return 0;
}
