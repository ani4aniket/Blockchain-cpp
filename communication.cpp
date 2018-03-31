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
    std::cout << "debug l12, " << sendto_ip.size() << " " << sendto_port.size() << std::endl;
    int sockfd, n;
    char buff[256], recvline[256];//+1?
    strcpy(buff, message.c_str());
    printf("message is %s, size is %d\n", buff, strlen(buff));
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << "debug l20" << std::endl;

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(sendto_port.c_str()));

    inet_pton(AF_INET, sendto_ip.c_str(), &servaddr.sin_addr);

    connect(sockfd, (sockaddr*)&servaddr, sizeof(servaddr));

    std::cout << "debug l30, write to " << sendto_ip << " " << sendto_port << std::endl;

    write(sockfd, buff, strlen(buff));

    close(sockfd);//if do not close it will block ?

    std::cout << "debug L35" << std::endl;
}

std::string receive_message(std::string listen_port)
{
    std::cout << "debug L42 " << listen_port << std::endl;

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

    std::cout << "debug L59" << std::endl;

    for(;;)
    {
        std::cout << "debug L63, listen on " << listen_port << std::endl;
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

            close(listenfd);

            std::cout << "debug L79" << std::endl;

            return str;
        }
    }
}