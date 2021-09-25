// Hamad Khan
// 02/26/2021
// Race Conditions Studio

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

int race = 0;

void* adder(void* var) {
  for (int i = 0; i <= 2000000; i++) {
    pthread_mutex_lock(var);
    race++;
    pthread_mutex_unlock(var);
  }
}

void* subtractor(void* var) {
  for (int i = 0; i <= 2000000; i++) {
    pthread_mutex_lock(var);
    race--;
    pthread_mutex_unlock(var);
  }
}

int main(int argc, char* argv[]) {

  int ret;
  pthread_t tid1, tid2;

  pthread_mutex_t m;
  pthread_mutex_init(&m, NULL);

  ret = pthread_create(&tid1, NULL, &adder, &m);
  if (ret != 0) {
    perror("Error creating thread");
    exit(-1);
  }

  ret = pthread_create(&tid2, NULL, &subtractor, &m);
  if (ret != 0) {
    perror("Error creating thread");
    exit(-1);
  }

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);

  printf("Value of race is: %d \n", race);

  return 0;
}
