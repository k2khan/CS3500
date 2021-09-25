// Hamad Khan
// 02/12/2021
// Pipe program


#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(int argc, char* argv[]) {
  
  int fd[2]; // fd[0] refers to read, fd[1] refers to write
  pipe(fd); 

  int c1_pid, c2_pid;
  (c1_pid = fork()) && (c2_pid = fork()); // Creates two children

  if (c1_pid == 0) {
    // Child 1
    dup2(fd[1], STDOUT_FILENO); // Close STDOUT_FILENO in child 1, replacing it with the write end of the pipe
    close(fd[0]); // Child 1 does not need to read from the pipe
    char* cmd = "./prog1";
    char* myargv[] = {"prog1", '\0'};
    int ret = execvp(cmd, myargv);
    if (ret == -1) {
      perror("Error executing program 1");
    }
  } else if (c2_pid == 0) {
    // Child 2
    dup2(fd[0], STDIN_FILENO); //Close STDIN_FILENO in child 2, replacing it with the read end of the pipe
    close(fd[1]); // Child 2 does not need to write to the pipe
    char* cmd = "./prog2";
    char* myargv[] = {"prog2", '\0'};
    int ret = execvp(cmd, myargv);
    if (ret == -1) {
      perror("Error executing program 2");
    }
  } else {
    // Parent does not need to read OR write to the pipe
    close(fd[1]);
    close(fd[0]);
    // Wait on the children to execute.
    waitpid(c2_pid, NULL, 0);
    waitpid(c1_pid, NULL, 0);
  }


  return 0;
}
