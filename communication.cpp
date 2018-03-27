#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

void send_message(const std::string& sendto_ip, const std::string& sendto_port, const std::string& message, const std::string& receive_ip, const std::string& receive_port)
{
    int sockfd, n;
    char buff[256], recvline[256];//+1?
    strcpy(buff, message.c_str());
    printf("message is %s, size is %d\n", buff, strlen(buff));
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));

    inet_pton(AF_INET, ip.c_str(), &servaddr.sin_addr);

    connect(sockfd, (sockaddr*)&servaddr, sizeof(servaddr));

    write(sockfd, buff, strlen(buff));
}

std::string receive_message(std::string listen_port)
{
    int listenfd, connfd, n;
    struct sockaddr_in servaddr;
    char buff[256], recvline[256];

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
        printf("accept sonnfd: %d\n", connfd);

        // n = read(connfd, recvline, 256);
        // printf("n is %d, %d, %s\n", n, strlen(recvline), recvline);

        while((n = read(connfd, recvline, 256) > 0))
        {
            printf("read: %d -- buff_szie : %d, buff: %s \n", n, strlen(recvline), recvline);
            // printf("debug line49");std::cout<<std::endl;
            std::string str(recvline);
            memset(recvline, 0, sizeof(recvline));

            return str;
        }
    }
}