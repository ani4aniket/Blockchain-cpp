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

        while((n = read(connfd, recvline, 256) > 0))
        {
            //printf("1. read: %d -- buff: %s \n", n, recvline);
            std::string str(recvline);
            std::string request = parse_request(str);
            std::string param = parse_param(str);
            std::map<std::string, int> map;
            map["mine"] = 0;
            map["new_transaction"] = 1;
            map["full_chain"] = 2;
            map["register_nodes"] = 3;
            map["consensus"] = 4;
            switch(map.find(request)->second)
            {
                case 0 :
                {
                    mine(param);
                    break;
                }
                case 1 :
                {
                    new_transaction(param);
                    break;
                }
                case 2 :
                {
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
                    printf("do noting!\n");
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

}

void App::full_chain()
{
    printf("full chain : %s\n", my_blockchain->get_chain());
}

void App::register_nodes(std::string param)
{

}

void App::consensus()
{

}

std::string App::parse_request(std::string str)
{

}

std::string App::parse_param(std::string str)
{

}