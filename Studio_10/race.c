// Hamad Khan
// 02/26/2021
// Race Conditions Studio

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

int race = 0;

void* adder(void* var) {
  for (int i = 0; i <= 20000000; i++) {
    race++;
  }
}

void* subtractor(void* var) {
  for (int i = 0; i <= 20000000; i++) {
    race--;
  }
}

int main(int argc, char* argv[]) {
  int ret;
  pthread_t tid1, tid2;
  
  ret = pthread_create(&tid1, NULL, &adder, NULL);
  if (ret != 0) {
    perror("Error creating thread");
    exit(-1);
  }

  ret = pthread_create(&tid2, NULL, &subtractor, NULL);
  if (ret != 0) {
    perror("Error creating thread");
    exit(-1);
  }

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);

  printf("Value of race is: %d \n", race);

  return 0;
}
