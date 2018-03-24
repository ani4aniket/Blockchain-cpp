#include "App.h"
#include "Blockchain.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <map>
#include <iostream>

App::App()
// : my_blockchain?
{
    my_blockchain = new Blockchain();
}

App::~App()
{
    delete my_blockchain;
}

void App::run()
{
    int listenfd, connfd, n;
    struct sockaddr_in servaddr;
    char buff[256], recvline[256];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(5000);

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
            // printf("debug line50");std::cout<<std::endl;
            std::string request = parse_request(str);
            // std::cout<<"debug line57"<<std::endl;
            std::string param = parse_param(str);
            // printf("debug line52");std::cout<<std::endl;
            std::map<std::string, int> map;
            map["mine"] = 0;
            map["new_transaction"] = 1;
            map["full_chain"] = 2;
            map["register_nodes"] = 3;
            map["consensus"] = 4;

            // std::cout<<"debug line66  "<<request<<std::endl;

            auto it = map.find(request);
            // printf("debug line60");std::cout<<std::endl;
            switch(it != map.end() ? it->second : -1)
            {
                case 0 :
                {
                    mine(param);
                    break;
                }
                case 1 :
                {
                    std::cout << "receive new_transaction request!" << std::endl;
                    new_transaction(param);
                    break;
                }
                case 2 :
                {
                    printf("receive full_chain request!");std::cout<<std::endl;
                    full_chain();
                    break;
                }
                case 3 :
                {
                    register_nodes(param);
                    break;
                }
                case 4 :
                {
                    consensus();
                    break;
                }
                default :
                {
                    printf("do noting!\n");std::cout<<std::endl;
                    break;
                }
            }
        }

        close(connfd);
    }
}

void App::mine(std::string param)
{
    Transaction* transaction = new Transaction("0", param, 1);
    my_blockchain->new_transaction(transaction);
    my_blockchain->new_block(my_blockchain->proof_of_work());
}

void App::new_transaction(std::string param)
{
    std::string sender(param, 0, param.find(" "));
    param.assign(param.find(" "), std::string::npos);
    std::string recipient(param, 0, param.find(" "));
    param.assign(param.find(" "), std::string::npos);
    int mount = atoi(param.c_str());
    //TODO
}

void App::full_chain()
{
    auto str_vector = my_blockchain->get_chain();
    for(auto it = str_vector.begin(); it != str_vector.end(); it++)
    {
        std::cout << *it << std::endl;
    }
}

void App::register_nodes(std::string param)
{

}

void App::consensus()
{

}

std::string App::parse_request(std::string str)
{
    return str.assign(str, 0, str.find(" "));
}

std::string App::parse_param(std::string str)
{
    return str.assign(str, str.find(" ") == std::string::npos ? str.size() : str.find(" "), std::string::npos);
}