// Hamad Khan
// 2/3/2021
// First program in c.

#include<unistd.h>

int main(int argc, char* argv[]){
  char buffer[] = "Hello, world! \n";
  write(STDOUT_FILENO, buffer, 100);
  return 0;
}
