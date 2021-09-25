// Hamad Khan
// 02/09/2021
// Copycat of linux's cat program


#include<unistd.h>
#define bufferSize 1024

char buffer[bufferSize];

int main(int argc, char* argv[]) {
  while (1) {
    ssize_t retVal = read(STDIN_FILENO, buffer, bufferSize);
    if (retVal == -1) {
      break;
    }

    write(STDOUT_FILENO, buffer, retVal);
    }
  return 0;
}
