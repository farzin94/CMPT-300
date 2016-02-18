#include<stdio.h>

#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<sys/wait.h>

#include<assert.h>

#define COMMAND_LENGTH 1024
#define NUM_TOKENS 513
#define PATH_MAX 4096
#define HISTORY_DEPTH 10

char history[HISTORY_DEPTH][COMMAND_LENGTH];

/**
 * Read a command from the keyboard into the buffer "buff" and
 * tokenize it such that tokens[i] points into buff to the i'th
 * token in the command.
 * buff: Buffer allocated by the calling code. Must be at least
 * COMMAND_LENGTH bytes long.
 * tokens[]: Array of character pointers which point into buff.
 * Must be at least NUM_TOKENS long. Will strip out up to one final
 * & token. tokens will be NULL terminated.
 * in_background: pointer to a boolean variable. Set to true if user
 * entered an & as their last token; otherwise set to false.
 */

 void add_history(char* tokens,char* buff, int count){
    int i=count;
    int j=0;
    //int k=0;
    if (strcmp(buff,"") != 0){
       if (strlen(buff)!=0){
          if(count>9) i=count-10;

          for(j=0; j<COMMAND_LENGTH; j++){
             history[i][j] = tokens[j];
            // k++;
          }

         //  for (int j=0; j<strlen(buff); j++){
         //     history[i][j] = buff[j];
         // }
       }
    }
 }

// void retrieve_history(char* buff, char* tokens[]){
//
//
//
// }

/**
 * TODO:
 * The numbers are being printed correctly i.e the last 10 numbers
 * are printed with correct numbering.
 * Corresponding commands are wrong. Weird shit comes out.
 * Im not sure if its print_history or add_history that does this.
 * I'll keep working on it for now.
 * @param count [description]
 * @param buff  [description]
 */
void print_history(char* tokens, int count, char *buff){

   int n=1;
   if(count>10){
      n=count-10;
   }
   while(n<=count){
      printf("%d\t", n);
      for(int j=0; j<sizeof(tokens); j++){
        printf ("%c", history[(n-1)%10][j]);
        //j++;
      }
      // for (int l=0; l<strlen(buff); l++){
      //  printf ("%c", history[(n-1)%10][l]);
      // }
      printf("\n");
      n++;
   }
}


int tokenize_command(char* buff, char* tokens[]){

   int i=0;

   char *local_buff;
   local_buff = strtok(buff, " \t\r\a");

   while(local_buff != NULL){
      tokens[i] = local_buff;
      i++;
      local_buff = strtok(NULL, " \t\r\a");
   }
   tokens[i] = NULL;

   return i;

}

void read_command(char *buff, char *tokens[], _Bool *in_background){

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
   //assert(strcmp(tokens[token_count - 1], "&") == 0);
   if (token_count > 0 && strcmp(tokens[token_count - 1], "&") == 0) {
      *in_background = true;
      tokens[token_count - 1] = 0;
   }
}

/**
 * Main and Execute commands
 */
int main(int argc, char*argv[]){

   char input_buffer[COMMAND_LENGTH];
   char *tokens[NUM_TOKENS];

   int count=0;
   int result;

   //For Built-in commands
   char *currentDir;
   char *tmpDir;
   char* cwd;
   char blah[PATH_MAX + 1];

   while (true){
      // Get command
      // Use write becausae we need to use read()/write() to work with
      // signals, and they are incompatible with print()
      currentDir = getcwd(NULL, 0);
      write(STDOUT_FILENO, currentDir, strlen(currentDir));
      write(STDOUT_FILENO, ">", strlen(">"));
      _Bool in_background = false;
      //printf("input_buffer1: %s ", input_buffer);
      read_command(input_buffer, tokens, &in_background);

      // int i=1;
      // strcpy(input_buffer, tokens[0]);
      // while (tokens[i]!=NULL){
      //
      // }
      //    printf("%s", input_buffer);

      if (strlen(input_buffer)!=0){
         add_history(tokens[0], input_buffer, count);
         //retrieve_history(input_buffer, tokens);
         count++;
      }

      if(tokens[0] != NULL){

   	 	if (strcmp(tokens[0], "exit") == 0){
   	 	   //printf("the current is %s\n", tokens[0]);
            return 0;
         }
         else if (strcmp(tokens[0], "pwd") == 0){
            cwd = getcwd(blah, (PATH_MAX + 1));
            printf("%s\n", cwd);
            continue;
         }
         else if (strcmp(tokens[0], "history") == 0){

            print_history(tokens[0], count, input_buffer);
         }
   		else if (strcmp(tokens[0], "cd") == 0){
            currentDir = (getcwd(NULL, 0));
            if (strchr(tokens[1], '/')){
        			chdir(tokens[1]);
        			printf( "Directory changed to %s\n", tokens[1]);
     		    }
        		else{
        			//currentDir = (getcwd(NULL, 0));
        			tmpDir = strcat(currentDir, "/");
        			tokens[1] = strcat(tmpDir, tokens[1]);
        			chdir(tokens[1]);
        		}
            continue;
 		   }
    	// 	else if (strcmp(tokens[0], "cd") == -1){
       //  		perror(tokens[1]);
         // }

         else {

            pid_t child = fork();

            if (child == 0){
               if (execvp(tokens[0], tokens) < 0) {
                  write(STDOUT_FILENO, "Error: exec failed\n", strlen("Error: exec failed\n"));
                }
               exit(-1);
            }

            else if(child < 0){
               write(STDOUT_FILENO, "Error: fork() failed\n", strlen("Error: fork() failed\n"));
            }
            else {
               if (in_background == false){
                  waitpid(child, &result, 0);
               }
            }
            while (waitpid(-1, NULL, WNOHANG)>0){
               ;
            }

         }
      }
      /**
       * Steps for Basic Shell:
       * 1. Fork a child process
       * 2. Child process invokes execvp() using results in token Array
       * 3. If in_background is false, parent waits for child to finish.
       * Otherwise, parent loops back to read_command() again immediately
       */

   }

   return 0;
}
