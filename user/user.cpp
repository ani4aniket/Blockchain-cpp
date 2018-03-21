#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char** argv)
{
    int sockfd, n;
    char buff[256], recvline[256];//+1?
    strcpy(buff, argv[2]);
    printf("argv[2] is %s, size is %d\n", buff, strlen(buff));
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5000);

    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    connect(sockfd, (sockaddr*)&servaddr, sizeof(servaddr));

    write(sockfd, buff, strlen(buff));

    exit(0);
}