#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

// Use 127.0.0.1 for <ip address>
int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Useage: ./client <ip address> <port number> \n");
        return 0;
    }
    char* ipAddr = argv[1];
    int portNum = atoi(argv[2]);

    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1) {
        handle_error("socket");
    } else {
        printf("Socket successfully created \n");
    }

    // Declare struct
    struct sockaddr_in address;

    // Set to 0
    memset(&address, 0, sizeof(struct sockaddr_in));
    address.sin_family= AF_INET;
    address.sin_port = htons(portNum);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    int retVal = inet_aton(ipAddr, &(address.sin_addr));
    if (retVal == 0) {
        printf("Error: IP address not valid \n");
        return -1;
    }
    
    if (connect(sfd, (struct sockaddr *) &address, sizeof(struct sockaddr_in)) == -1) {
        handle_error("connect");
    } else {
        printf("Connected to the server \n");
    }
    
    // Handle read/write
    write(sfd, "Hello from the client.", 23);

    FILE* sockStream = fdopen(sfd, "r+");

    char* username = "David";
    int X = 42;
    fprintf(sockStream, "My name is %s and my favorite number is %d \n", username, X);

    close(sfd);

    return 0;
}
