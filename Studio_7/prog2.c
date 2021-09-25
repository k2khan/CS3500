// Hamad Khan
// 02/12/2021
// Program 2

#include<stdio.h>

int main(int argc, char* argv[]) {
  char inputString[1024];
  char* res;
  
  while (1) {
  res = fgets(inputString, sizeof inputString, stdin);
  if (res == NULL) {
    break;
    }
  printf("The program got: %s \n", res);
  }

  return 0;
}
