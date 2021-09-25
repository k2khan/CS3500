// Hamad Khan
// 02/07/2021
// Sample Program

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

char* reverseString(char* input) {
  //Count the amount of characters in input string
  int inc = 0;
  char* output = (char*)malloc(inc + 1);

  while (input[inc] != NULL) {
    inc++;
  }

  int counter = 0;
  //Reverse
  for(int i = 0; inc > i; inc--) {
    output[counter] = input[inc - 1];
    counter++;
  }
  output[counter+1] = '\0';
  return output;
}


void printReverse(char *string) {
  int inc = 0;
  while (string[inc] != NULL) {
    inc++;
  }
  //Reverse
  for(int i = 0; inc > i; inc--) {
    printf("%c \n", string[inc - 1]);
  }
}


int main(int argc, char* argv[]) {
  char *messagePtr = "HELLOWORLD!";
  printf("%s\n", messagePtr);
  
  printf("Printing by dereferencing using index: \n");
  for (int i = 0; i < 11; i++) {
    printf("%c\n", messagePtr[i]);
  }

  printf("%c\n", *messagePtr);

  printf("%c\n", *(messagePtr+1));
  
  printf("Printing using pointer arithmetic: \n");
  for (int i = 0; i < 11; i++) {
    printf("%c\n", *(messagePtr + i));
  }
  
  printf("Printing using pointer while loop: \n");
  int inc = 0;
  while (messagePtr[inc] != NULL) {
    printf("%c\n", messagePtr[inc]);
    inc++;
  }
  
  printf("Printing in reverse: \n");
  printReverse(messagePtr);

  char* reversedMessage = reverseString(messagePtr);
  printf("Reversed string: %s\n", reversedMessage);

  return 0;
}
