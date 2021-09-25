// Hamad Khan
// 03/03/2021
// Atomics

#include<stdio.h>

int main(int argc, char* argv[]) {
  int var = 0;
  printf("The value of the variable : %d \n", var);

  int newVar = __sync_add_and_fetch(&var, 1);
  printf("The value of the variable is now: %d \n", newVar);

  return 0;
}
