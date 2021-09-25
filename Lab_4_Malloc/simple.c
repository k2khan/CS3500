#include <stdio.h>
#include <stdlib.h>

int main(){

	void *ptr, *ptr2;

	ptr = malloc(2000);
	ptr2 = malloc(2000);

	printf("Ptr : %p\n", ptr);
	printf("Ptr2: %p\n", ptr2);
	printf("Diff: %lld\n", (long long int)ptr2-(long long int)ptr);

	free(ptr);
	free(ptr2);

	ptr = malloc(500);
	ptr2 = malloc(500);

	printf("Ptr : %p\n", ptr);
	printf("Ptr2: %p\n", ptr2);
	printf("Diff: %lld\n", (long long int)ptr2-(long long int)ptr);

	free(ptr);
	free(ptr);
	
	ptr = malloc(4000);
	ptr2 = malloc(4000);

	printf("Ptr : %p\n", ptr);
	printf("Ptr2: %p\n", ptr2);
	printf("Diff: %lld\n", (long long int)ptr2-(long long int)ptr);

	free(ptr);
	free(ptr2);

	return 0;
}


