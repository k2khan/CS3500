// Hamad Khan
// 02/10/2021
// Studio 5 program

#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(int argc, char* argv[]) {
  int pid = fork();

  if (pid > 0) {
    waitpid(pid, NULL, 0);
    printf("The parent's processor id is: %i \n", getpid());
  }

  char* child_argv[] = {"ls", "-l", NULL};
  char* child_prog = child_argv[0];

  if (pid == 0) {
    sleep(3); 
    printf("The child's processor id is: %i \n", getpid());
    execvp(child_prog, child_argv);
  }

  return 0;
}
