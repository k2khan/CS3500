// Hamad Khan
// 02/12/2021
// User Input

#include<stdio.h>
#include<string.h>
#include<unistd.h>


int main(int argc, char* argv[]) {
  char input_string[1024];
  char* token;
  
  /*
  //Get user input
  printf("Please enter some input: ");
  fgets(input_string, sizeof input_string, stdin);
  printf("Your input was: %s", input_string);

  //Strip the newline character
  input_string[strcspn(input_string, "\n")] = 0;
  
  //Print the first token
  token = strtok(input_string, " ");
  //printf("The first token is: %s \n", token);

  //Print remaining tokens
  while (token != NULL) {
    //printf("The current token is: %s \n", token);
    token = strtok(NULL, " ");
  }
  **/
  
  //Mimicking executing commands in shell
  int max_args = 15;
  int max_argv_size = max_args + 2;
  char* cmd;
  char* my_argv[max_argv_size];
  char* res;
  int counter = 1;
  
  //Get user input and remove new line character
  printf("Please enter some input: ");
  fgets(input_string, sizeof input_string, stdin);
  input_string[strcspn(input_string, "\n")] = 0;

  //Save first command
  cmd = strtok(input_string, " ");
  my_argv[0] = cmd;

  //Save rest of the commands
  while (1) {
    res = strtok(NULL, " ");
    if (res == NULL) {
      my_argv[counter] = '\0';
      break;
    }
    my_argv[counter] = res;
    counter++;
  }
  
  //Execute command
  execvp(cmd, my_argv);

  return 0;
}

