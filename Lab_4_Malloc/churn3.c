#include<stdio.h>
#include<stdlib.h>
extern void* mymalloc(size_t request);

int main(int argc, char* argv[]) {
    void* ret = mymalloc(1000);
    printf("Ret is: %p", ret);
    return 0;
}