#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>

void send_message(const std::string& sendto_ip, const std::string& sendto_port, const std::string& message, const std::string& receive_ip, const std::string& receive_port)
{
    int sockfd, n;
    char buff[4096], recvline[4092];//+1?
    strcpy(buff, message.c_str());
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(sendto_port.c_str()));

    inet_pton(AF_INET, sendto_ip.c_str(), &servaddr.sin_addr);

    connect(sockfd, (sockaddr*)&servaddr, sizeof(servaddr));

    write(sockfd, buff, strlen(buff));

    close(sockfd);//if do not close it will block ?

}

std::string receive_message(std::string listen_port)
{
    int listenfd, connfd, n;
    struct sockaddr_in servaddr;
    char buff[4092], recvline[4092];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(listen_port.c_str()));

    bind(listenfd, (sockaddr*)&servaddr, sizeof(servaddr));

    listen(listenfd, 1024);

    for(;;)
    {
        connfd = accept(listenfd, (sockaddr*)NULL, NULL);

        while((n = read(connfd, recvline, 4092) > 0))
        {
            std::string str(recvline);
            memset(recvline, 0, sizeof(recvline));

            close(listenfd);

            return str;
        }
    }
}