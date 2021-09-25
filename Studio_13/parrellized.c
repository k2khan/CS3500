// Hamad Khan
// 03/06/2021
// OpenMP

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<stdbool.h>
#include<omp.h>

int isPrime(int num) {
  #pragma omp parallel for
  for (int i = 2; i <= num; i++) {
    int prime = 1;
    for (int j = 2; j <= i/2; j++) {
      if (i % j == 0) {
        prime = 0;
        break;
      }
    }
    if (prime == 1) {
    }
  }
  return 0;
}

int main(int argc, char* argv[]) {
  int prime = atoi(argv[1]);
  int ret = isPrime(prime);
  
  /*
  if (ret == 1) {
    printf("The number %d is prime. \n", prime);
  }

  if (ret == 0) {
    printf("The number %d is not prime. \n", prime);
  }
  **/



  return 0;
}
