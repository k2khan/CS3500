#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("Failed to create socket");
        return -1;
    }

    char *path = "/somepath";
    struct sockaddr_un sockaddr;
    memset(&sockaddr, 0, sizeof(struct sockaddr_un));
    sockaddr.sun_family = AF_UNIX;
    memcpy(&sockaddr.sun_path, path, strlen(path));

    if (connect(sockfd, (struct sockaddr *)&sockaddr, sizeof(struct sockaddr_un)) == -1)
    {
        printf("Failed to connect");
        return -1;
    }
    printf("Connected to server\n");
    char buffer[1000];
    memset(buffer, 0, 1000);
    int size = read(STDIN_FILENO, buffer, 1000);
    buffer[size - 1] = 0;
    write(sockfd, buffer, size);
    while (size != 0)
    {
        size = read(sockfd, buffer, 1000);
        write(STDOUT_FILENO, buffer, size);
    }

    return 0;
}
