#include <sys/socket.h> 
#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

int sockfd;
int quit = 0;
char* termchar = "/0";

void* getOutput(void* args);
void* getInput(void* args);

void* getOutput(void* args){
    while(quit == 0){
        char msg[1024];
        fgets(msg, 1024, stdin);
        if(strncmp(msg, "quit", 4)){
            quit = 1;
            shutdown(sockfd, SHUT_RDWR);
            break;
        } // end strncmp()
        printf("I am sending %s\n", msg);
        send(sockfd, msg, strlen(msg + 1), 0);
    } // end while(quit == 0)
} // end getOutput()

void* getInput(void* args){
    char inbuf[1024];
    while(quit == 0){
        inbuf[0] = termchar; // set buffer to /0
        int readret = recv(sockfd, inbuf, 1024, MSG_DONTWAIT);
        if(readret == -1){
            if(errno == EAGAIN || errno == EWOULDBLOCK){
                // do nothing
            }
            else{
                printf("server quit unexpectedly");
                quit = 1;
                shutdown(sockfd, SHUT_RDWR);
                break;
            }
        }
        else{
            inbuf[readret] = termchar; // set last byte of buffer following read input to /0
            if(inbuf[0] != termchar){ // while "inbuf" buffer is not empty
                printf("%s", inbuf);
            } // end if
        } // end else
    } // end while(quit == 0)
} // end getInput()

// gcc -o client client.c -lpthread
// Use 127.0.0.1 for <ip address>
int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Useage: ./client <ip address> <port number> \n");
        return 0;
    }
    char* ipAddr = argv[1]; // get ip address from value at index 1 of argument list
    int portNum = atoi(argv[2]); // get port number from value at index 2 of argument list
    // NOTE: atoi() makes integer from char*

    // step 1 - create a socket
    sockfd = socket(AF_INET, SOCK_STREAM , 0);
    if (sockfd == -1) {
        handle_error("socket");
    } else {
        printf("Socket successfully created \n");
    }
    // bind port number to socket
    struct sockaddr_in address;
    memset(&address, 0, sizeof(struct sockaddr_in)); // initialize memory to zero
    address.sin_family= AF_INET; 
    address.sin_port = htons(portNum); // bind port number
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    int retVal = inet_aton(ipAddr, &(address.sin_addr));
    if (retVal == 0) {
        printf("Error: IP address not valid \n");
        return -1;
    }

    // step 2 - connect to a remote peer
    
    if (connect(sockfd, (struct sockaddr *) &address, sizeof(struct sockaddr_in)) == -1) {
        handle_error("connect");
    } else {
        printf("Connected to the server \n");
    }

    // start threading
    pthread_t output;
    pthread_t input;

    if(pthread_create(&output, NULL, getOutput, NULL) != 0){
        handle_error("could not create output thread");
    }
    if(pthread_create(&input, NULL, getInput, NULL) != 0){
        handle_error("could not create input thread");
    }

    if(pthread_join(output, NULL) != 0){
        handle_error("could not join output thread");
    }
    if(pthread_join(input, NULL) != 0){
        handle_error("could not join input thread");
    }

    close(sockfd);
    return 0;
}

