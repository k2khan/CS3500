  // Hamad Khan
// 03/06/2021
// OpenMP

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<omp.h>

int isPrime(int num) {
  #pragma omp parallel for schedule(dynamic, 1)
  for (int i = 0; i <= num; i++) {
    printf("Iteration %d: Thread: %d \n", i, omp_get_thread_num());
      if (i <=4) {
        sleep(1);
      }
  }
  return 0;
}

int main(int argc, char* argv[]) {
  int numAmount = atoi(argv[1]);
  int ret = isPrime(numAmount);
  omp_set_num_threads(5);
  
  return 0;
}
