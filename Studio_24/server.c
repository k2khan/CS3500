// Credits to the example from "man 2 bind"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define LISTEN_BACKLOG 50
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)


int main(int argc, char *argv[]) {
    // Get port number
    if (argc != 2) {
        printf("Useage: ./server <port number>");
        return 0;
    }
    int portNum = atoi(argv[1]);

    // Create socket
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1) {
        handle_error("Unable to create socket \n");
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
    
    // Bind the address
    if (bind(sfd, (struct sockaddr *) &address, sizeof(struct sockaddr_in)) == -1) {
        handle_error("Unable to bind \n");
    } else {
        printf("Socket successfully binded \n");
    }

    // Set the port to listening
    if (listen(sfd, LISTEN_BACKLOG) == -1) {
        handle_error("Couldn't listen");
    } else {
        printf("Server listening \n");
    }

    /* Now we can accept incoming connections one
        at a time using accept(2) */
    int cfd = accept(sfd, NULL, 0);
    if (cfd == -1) {
        handle_error("Couldn't accept");
    } else {
        printf("Server has accepted");
    }

    /* Code to deal with incoming connection(s)... */
    char buffer[1024];
    memset(buffer, 0, 1024);
    int bytesRead = read(cfd, buffer, 1024);
    buffer[bytesRead] = '\0';

    printf("Client: %s \n", buffer);

    FILE* sockStream = fdopen(cfd, "r+");
    fgets(buffer, 1024, sockStream);

    printf("Client: %s \n", buffer);


    /* When no longer required, the socket pathname, MY_SOCK_PATH
        should be deleted using unlink(2) or remove(3) */
}