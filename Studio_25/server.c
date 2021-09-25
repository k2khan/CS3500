#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

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

    if (bind(sockfd, (struct sockaddr *)&sockaddr, sizeof(struct sockaddr_un)) == -1)
    {
        printf("Failed to bind\n");
        return -1;
    }

    if (listen(sockfd, 1) == -1)
    {
        printf("Failed to listen");
        return -1;
    }

    while (1)
    {
        int cfd = accept(sockfd, NULL, NULL);
        if (cfd == -1)
        {
            printf("Failed to accept");
            return -1;
        }
        // get input
        int bufferSize = 1024;
        char buffer[bufferSize];
        int bytesRead = read(cfd, buffer, bufferSize);

        // quit
        if (strcmp("quit", buffer) == 0)
            break;

        // ls
        if (strcmp("ls", buffer) == 0)
        {
            FILE *ls = popen("ls", "r");
            fgets(buffer, bufferSize, ls);
            printf("%s", buffer);
            write(cfd, buffer, strlen(buffer) + 1);
            fclose(ls);
            continue;
        }

        //open file
        int fd = open(buffer, O_RDONLY);
        if (fd < 0)
        {
            char *message = "Failed to open file\n";
            perror("Failed to open file");
            write(cfd, message, strlen(message) + 1);
            shutdown(cfd, SHUT_RDWR);
            continue;
        }

        //read file
        int size = bufferSize;
        while (size == bufferSize)
        {
            char buffer[bufferSize];
            size = read(fd, buffer, bufferSize);
            write(cfd, buffer, size);
        }
        close(fd);
        shutdown(cfd, SHUT_RDWR);
    }

    unlink(sockaddr.sun_path);
    return 0;
}
