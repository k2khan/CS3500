// Hamad Khan
// 02/23/2021
// Thread studio

#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct data_pipe {
  int arg1;
  int ret;
  char arg2[15];
};

void* thread_entry(void* args) {
  struct data_pipe *arg_ptr = (struct data_pipe*) args;

  printf("Thread arg2: %s \n", arg_ptr->arg2);
  printf("Thread arg1: %d \n", arg_ptr->arg1);
  arg_ptr->ret = 2048;
  printf("Thread ret: %d \n", arg_ptr->ret);

  return NULL;
}

int main(int argc, char* argv[]) {

  /*
  struct data_pipe pipe;
  pipe.arg1 = 1;
  strncpy(pipe.arg2, "Hello, World!", 15);
  pipe.ret = 2;

  struct data_pipe *struct_ptr = &pipe;
  
  int retval;
  pthread_t ptid;
  retval = pthread_create(&ptid, NULL, &thread_entry, struct_ptr);
  if (retval != 0) {
    printf("Error creating thread! \n");
    exit(-1);
  }
  
  retval = pthread_join(ptid, NULL);
  if (retval != 0) {
    printf("Error creating thread! \n");
    exit(-1);
  }

  printf("After executing pthread_join, the value of arg1 is: %d \n", pipe.arg1);
  */ 
  
  // For question 8
  unsigned bufSize = 100;
  int num_threads = 5;

  struct data_pipe pipe[num_threads];
  pthread_t thread[num_threads];

  for (int i = 0; i < num_threads; i++) {
    pipe[i].arg1 = 1024;
    pipe[i].ret = 1;

    struct data_pipe *struct_ptr = &pipe[i];
    pthread_create(&thread[i], NULL, &thread_entry, struct_ptr);
    snprintf(pipe[i].arg2, bufSize, "This is thread %d", i);
    pthread_join(thread[i], NULL);
  }

    

  return 0;
}
