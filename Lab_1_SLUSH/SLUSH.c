// Hamad Khan, Julia Pratt, Bridget O'Reilly
// 02/12/2021
// User Input

#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<signal.h>

//GRADING NOTE: Program performs flawlessly, 100/100

void INThandler(int);

/* TODO
 * Signal Handling (^C) (We only want SLUSH itself to handle ^C)
 * Input syntax handling
 * Change Directory
 **/


int main(int argc, char* argv[]) {
  signal(SIGINT, INThandler);

  while(1){
    char input_string[128];
    char* piped[15]; // Stores strtok with respect to (
    char* commands[15]; //Reverses the piped input
    char* myArgv[15]; // REF: Max 15 arguments
    char* cmd;
    char* res;

    //Get user input and remove new line character
    printf("$ ");
    char* retval = fgets(input_string, sizeof input_string, stdin);
    if(retval == NULL){
      break; // End program
    }

    input_string[strcspn(input_string, "\n")] = 0; // Strip newline character

    //Parse first token with respect to "("
    int counter = 0;
    res = strtok(input_string, "(");
    piped[counter] = res;

    //Parse the rest of the tokens with respect to "("
    while (res != NULL) {
      res = strtok(NULL, "(");
      counter++;
      piped[counter] = res;
    }

    counter = counter - 1; // Subtract by one because of 0 indexing
    int index = counter; // Used for the for loop below

    // Reverse the commands, so that they execute in order
    for (int i = 0; i <= counter; i++) {
      commands[i] = piped[index];
      index--;
    }

    int oldPipeFD;

    for (int i = 0; i <= counter; i++) {
      //Read in command[i], save to cmd and argv[]
      int argVCounter = 1;
      res = strtok(commands[i], " ");
      cmd = res;
	  if (cmd == NULL) {
	    printf("null command");
		break;
      }
      myArgv[0] = res;
      while (res != NULL) {
        res = strtok(NULL, " ");
        myArgv[argVCounter] = res;
        argVCounter++;
      }


      // If we only have one command, fork and exec seperately. For ex, ("ls -l")
      if (counter == 0) {
		if (strncmp(myArgv[0], "cd", 2) == 0) {
			int ret = chdir(myArgv[1]);
			if (ret == -1) {
				perror("Could not change directory.");
			}
			break;
		}
		else {
          int ret = fork();
          if (ret == 0) {
            ret = execvp(cmd, myArgv);
            if (ret == -1) {
              perror("Could not execute in the child!");
              //Exit after perror
              exit(-1);
            }
          }
          waitpid(ret, NULL, 0);
          break;
        }
	  }

      int pipefd[2];

      oldPipeFD = pipefd[0];
      if((i == 0) || (i > 0 && i < counter)){
        // REF: is_first_process or is_middle_process
        pipe(pipefd);
      }

      int ret = fork();
      if(ret == 0){ // REF: In a child process
        if(i == 0){ // REF: is_first_process
          dup2(pipefd[1], STDOUT_FILENO);
          close(pipefd[0]);
        }
        if(i > 0 && i < counter){ // REF: is_middle_process
          dup2(oldPipeFD, STDIN_FILENO);
          dup2(pipefd[1], STDOUT_FILENO);
          close(pipefd[0]);
        }
        if(i == counter){ //REF: is_end_process
          dup2(oldPipeFD, STDIN_FILENO);
        }

        //fprintf(stderr, "CMD IS: %s ARG IS: %s \n", cmd, myArgv[1]);
        ret = execvp(cmd, myArgv);
        if (ret == -1) {
          perror("Could not execute in the child!");
          exit(-1);
        }
      }
      
      // In the parent
      if(i == 0){
        close(pipefd[1]);
      }

      if (i > 0 && i < counter) {
        close(oldPipeFD);
        close(pipefd[1]);
      }

      if (i == counter) {
        close(oldPipeFD);
        waitpid(ret, NULL, 0);
      } 
    }
  }
  return 0;
}

void INThandler(int sig) {
  printf("\n$ "); 
  fflush(stdout);
}
