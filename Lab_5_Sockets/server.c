// Credits to the example from "man 2 bind"

#define _GNU_SOURCE // man accept4

#include <sys/types.h>
#include <sys/socket.h>

#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>

// for linked list
struct Client {
    char name[100];
    int sock;
    struct Client* next;
};

struct Client* head = NULL; // initialize head to NULL
struct Client* client = NULL;
int clientnum = 0;

// https://www.geeksforgeeks.org/linked-list-set-3-deleting-node/

int delNode(struct Client** head, int sfd){
    // store head and create prev (?)
    struct Client *temp = *head, *prev;

    // if client list is empty
    if(head == NULL){ // empty list
        printf("cannot remove from empty list\n");
        return -1; // return error
    }
    // if head is what we want
    else if(temp != NULL && temp->sock == sfd){
        head = temp->next; // set head to second node
        free(temp); // free memory
        return 0; // return success
    }
    // otherwise, search for sfd and keep track of prev node
    while(temp != NULL && temp->sock != sfd){
        prev = temp; // keep track of prev node
        temp = temp->next; // iterate through list
    }
    // couldn't find client
    if(temp == NULL){ // we made it through the list and didn't find sfd
        printf("client not in list");
        return -1; // return error
    }
    
    prev->next = temp->next; // unlink node
    free(temp); // free memory allocated to temp
}

// Hamad's fancy error handling
#define LISTEN_BACKLOG 50
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char *argv[]) {
    // Get port number
    if (argc != 2) {
        printf("Useage: ./server <port number>\n");
        return 0;
    }
    int portNum = atoi(argv[1]); // set port num equal to the value at the first index of the argument list

    // step 1 - create socket
    int sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    // AF_INET --> IPv4 Internet protocols
    // SOCK_STREAM --> sequenced, reliable, two-way, connection-based byte streams (man 2 socket)
    // SOCK_NONBLOCK --> calling process will not wait
    // 0 --> has no protocol type (no protocol options for internet options)
    if (sockfd == -1) { // error handling
        handle_error("Unable to create socket \n");
    } else {
        printf("Socket successfully created \n");
    }

    // step 2 - bind port number to socket
    struct sockaddr_in address;
    memset(&address, 0, sizeof(struct sockaddr_in));
    address.sin_family= AF_INET; // IPv4 Internet protocols
    address.sin_port = htons(portNum); // set port to port number defined by argument list
    address.sin_addr.s_addr = htonl(INADDR_ANY); 
    
    // Bind the address
    if (bind(sockfd, (struct sockaddr *) &address, sizeof(struct sockaddr_in)) == -1) {
        handle_error("Unable to bind \n");
    } else {
        printf("Socket successfully binded \n");
    }

    // Set the port to listening
    if (listen(sockfd, LISTEN_BACKLOG) == -1) {
        handle_error("Couldn't listen\n");
    } else {
        printf("Server listening \n");
    }

    while (1) {
        char buffer[1024]; // used later
        memset(buffer, 0, 1024);
        // accept is non-blocking
        int clientsock = accept4(sockfd, NULL, 0, SOCK_NONBLOCK | SOCK_CLOEXEC);
        if (clientsock == -1) { //error
            if(errno == EAGAIN || errno == EWOULDBLOCK){ //check for benign errors
                // do nothing - compiler knows
            }
            else{
                handle_error("server could not accept client socket\n"); // handle malignant errors
            }
        }
        else if(clientsock >= 0){
            printf("server has accepted.\n");
            // add client to a linked list of clients managed in serverRecords.c
            // client is declared above to make it global
            client = (struct Client *) malloc(sizeof(struct Client));
            client->sock = clientsock; //make the client

            // set client name
            char name[1034];
            memset(name, 0, 1024);
            char numbuffer[1024];
            memset(numbuffer, 0, 1024);
            clientnum++;
            snprintf(numbuffer, 1024, "%d", clientsock);
            snprintf(name, 1034, "user%s", numbuffer);
            snprintf(client->name, 1034, "%s", name);
            // name is set now
            
            // this adds the node to the list
            if(!head){ // if head is NULL 
                // make new client into head of linked list
                head = client;
                head->next = NULL;
            }
            else{ // if head is not NULL
                // add new client to linked list as head
                struct Client* temp = head; //temp is head
                head = client; //head becomes the new client
                client->next = temp; //the new head points to the previous head
            }        
            printf("%s has connected\n", client->name);

            //sends connection message is someone has connected
            struct Client* current = head; //starting with newest client
            while(current){ //going to all clients in the list
                buffer[0] = "\0";
                snprintf(buffer, 1024, "%s has connected to the server\n", client->name); //client is the newest client
                send(current->sock, buffer, strlen(buffer), 0); // goes to current client

                current = current->next; // changes current client to the next node
            }  
        } // end else if(clientsock > 0)

        //regardles of successful connection or not, this happens. (user is already added.)
        struct Client* temp = head; //temp is the newest user, head
        while(temp){ //while there are still users in the list
            buffer[0] = "\0";
            int readret = recv(temp->sock, buffer, 1024, MSG_DONTWAIT); // read from socket to buffer
            printf("just read: %s\n", buffer);
            
            // if error - detect if benign or not (like with accept4)
            // if real error - remove client from linked list
            if(readret == -1){
                if(errno == EAGAIN || errno == EWOULDBLOCK){ //benign error
                    // do nothing
                }
                else{ // if returns -1 and does not set specified benign errno (real error)
                    handle_error("could not read\n");
                }
            } // end error handling
            else{ // if we successfully read
                if(strncmp(buffer, "quit ", 5) == 0){ // if quit command
                    close(temp->sock);
                } 
                else if(strncmp(buffer, "name ", 5) == 0 && strlen(buffer) > 5){ // if name command is entered followed by a requested name
                    char *newname = buffer + 5; // set newname equal to the contents of buffer following name
                    char *oldname = temp->name; // set oldname equal to the previous client name
                    snprintf(temp->name, strlen(newname), "%s", newname); // set client name to newname
                    // omnisend(("%s has changed name to %s\n", oldname, newname)); // send message to other clients
                    snprintf(buffer, 1034, "%s has changed name to %s", oldname, newname);
                    //sends connection message is someone has connected
                    struct Client* current = head; //starting with newest client
                    while(current){ //going to all clients in the list
                        send(current->sock, buffer, 1034, 0); // goes to current client
                        current = current->next; // changes current client to the next node
                    }  
                }
                else{ //something is read and it isnt quit or name
                    //sends connection message is someone has connected
                    struct Client* current = head; //starting with newest client
                    printf("you got mail!\n"); 
                    printf("the mail from %s reads: %s\n,",client->name, buffer);
                    while(current){ //going to all clients in the list
                                            
                        send(current->sock, buffer, 1034, 0); // goes to current client
                        current = current->next; // changes current client to the next node
                    }  
                }
            } // end else for read()
            temp = temp->next; // iterate through client list
        } // end while(temp)
    // delNode(*head, clientsock);
    // close(clientsock);
    sleep(0.5); 
    } // end big while loop
    
    // close(sockfd); // close server socket
    return 0;
} // end main
    