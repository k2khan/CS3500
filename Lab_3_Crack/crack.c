// Hamad Khan, Julia Pratt, Bridget O'Reilly
// 03/09/2021
// Lab 03 - Brute Force Password Cracking

//https://stackoverflow.com/questions/45621051/how-to-iteratively-generate-all-possible-combinations-of-letters-and-numbers-for
//https://stackoverflow.com/questions/6205195/given-a-starting-and-ending-indices-how-can-i-copy-part-of-a-string-in-c

#include<stdio.h>
#include<crypt.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include<pthread.h>

void iterate(char* str);
char* isFinish(char* passwdCombo, char* salt, char* targetHash, struct crypt_data* data);
char* encrypt(char* password, char* salt);
void* thread_entry(void* args);

//GRADING NOTES:
//
// Program conforms to all expectations- 
//
// - Includes usage statement
// - Finds size 4 and 5 test passwords in a reasonable time
// - Demonstrates parallel speedup
// - Good responses to questions
//
// - +4 points extra credit
//
// Score: 104/100

struct thread_args {
    int threads;
    int keySize;
    char* targetHash;
    char* salt;
    char start;
    char end;
};

int main(int argc, char* argv[]) {
    struct thread_args arguments;

    if (argc == 1) {
        printf("Useage: crack <threads> <keysize> <target> \n");
        exit(0);
    }

    if (argc != 4) {
        printf("Inorrect useage. Please type './crack' to find the correct useage statement. \n");
        exit(0);
    }

    arguments.threads = atoi(argv[1]);
    arguments.keySize = atoi(argv[2]);
    arguments.targetHash = argv[3];

     if (arguments.threads <= 0 || arguments.threads > 26) {
        printf("<threads> must be between 1 and 26 \n");
        exit(-1);
    }

    if (arguments.keySize <= 0 || arguments.keySize > 8) {
        printf("<keysize> must be between 0 and 8 \n");
        exit(-1);
    }

    char salt[3];
    strncpy(salt, &arguments.targetHash[0], 2);
    salt[2] = '\0';
    arguments.salt = salt;
    
    //set up threads
    struct thread_args pipe[arguments.threads];
    pthread_t threads[arguments.threads];

    int totalChars = 26;
    int numThreads = arguments.threads;
    int currChar = 96;

    for (int i = 0; i < arguments.threads; i++) {
        if (numThreads > 0) {
            int intDiv = totalChars / numThreads; // we get 6
            totalChars -= intDiv;
            pipe[i].start = currChar + 1;
            pipe[i].end = pipe[i].start + intDiv - 1;
            pipe[i].keySize = arguments.keySize;
            pipe[i].targetHash = arguments.targetHash;
            pipe[i].salt = arguments.salt;
        }
        if (pipe[i].end > 122) {
            pipe[i].end = 122;
        }
        pthread_create(&threads[i], NULL, thread_entry, &pipe[i]);
        numThreads--;
        currChar = pipe[i].end;
    } 

    for (int i = 0; i < arguments.threads; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}


void* thread_entry(void* args) {
    struct thread_args* argptr = args; // get the values from our struct
    struct crypt_data myData;
    myData.initialized = 0;

    // test all keysizes
    for (int key = 0; key < argptr->keySize; key++) {
        // iterate through all possible combinations of a-z (lowercase) of size keySize
        char* passwdCombo = (char*)malloc(sizeof(char)*(key + 1));
        memset(passwdCombo, argptr->start, key + 1);
        passwdCombo[key + 1] = '\0';
        //printf("The curr passwordcombo is %s \n", passwdCombo);

        // string to break if we get to the max passwdCombo
        char* stringToTest = (char*)malloc(sizeof(char)*(key + 1));
        memset(stringToTest, 'z', key + 1);
        stringToTest[0] = argptr->end;
        stringToTest[key + 1] = '\0';
        //printf("The string to test: %s \n", stringToTest);
        while(1) { 
            if(isFinish(passwdCombo, argptr->salt, argptr->targetHash, &myData)) {
                break;
            }

            // test all combinations
            iterate(passwdCombo);
            //printf("The passwordCombo is: %s \n", passwdCombo); 
            //we know it's the last possible combination if it is all z's, length of keysize
            //if passwdCombo == z's * length of keysize
            if (!strcmp(passwdCombo, stringToTest)) {
                break;
            }
        }
    }
}

// this code structure for string permutations inspired by https://stackoverflow.com/questions/10159649/generate-all-letter-combinations
char* isFinish(char* passwdCombo, char* salt, char* targetHash, struct crypt_data* data){
    char* res = crypt_r(passwdCombo, salt, data);
    if (strcmp(targetHash, res) == 0) {
        printf("The password is: %s\n", passwdCombo);
        return passwdCombo;
    }
    else {
        return NULL;
    }
}

void iterate(char *str){
    int index, carry;
    for(index = strlen(str)-1;index>=0;--index){
        if(str[index] == 'z'){
            carry = 1;
            str[index] = 'a';
        } else {
            carry = 0;
            str[index] += 1;
        }
        if(carry == 0)break;
    }
}
