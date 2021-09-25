// Hamad Khan
// 02/10/2021
// Studio 4 program

#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdio.h>

#define bufferSize 258

int main(int argc, char* argv[]) {
  char* inputFile = argv[1];
  char* buffer[bufferSize];
  
  //Open file descriptor
  int inFD = open(inputFile, O_RDONLY);
  if (inFD == -1) {
    perror("Could not open the file!");
    exit(-1);
  }
  
  while (1) {
    //Read the file descriptor
    ssize_t retVal = read(inFD, buffer, bufferSize);
    if (retVal == -1) {
      perror("Could not read the file!");
      exit(-1);
    }

     if (retVal == 0) {
      break;
    }
   
    //Write to standard output
    retVal = write(STDOUT_FILENO, buffer, retVal);
    if (retVal == -1) {
      perror("Could not write to file!");
      exit(-1);
    }
  }

  return 0;
}
