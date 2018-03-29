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

void send_message(const std::string& sendto_ip, const std::string& sendto_port, const std::string& message, const std::string& receive_ip, const std::string& receive_port);

App::App()
// : my_blockchain?
{
    my_blockchain = new Blockchain();
}

App::~App()
{
    delete my_blockchain;
}

void App::run(std::string listen_port)
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
            // printf("debug line50");std::cout<<std::endl;
            std::string request = parse_request(str);
            // std::cout<<"debug line57, request is "<<request<<std::endl;
            std::string param = parse_param(str);
            // std::cout<<"debug line60, param is "<<param<<std::endl;
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
                    std::cout << "receive mine request!" << std::endl;
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
                    std::cout << "receive register_nodes request!" << std::endl;
                    register_nodes(param);
                    break;
                }
                case 4 :
                {
                    std::cout << "receive consensus request!" << std::endl;
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
    // Transaction* transaction = new Transaction("0", param, 1);
    // my_blockchain->new_transaction(transaction);
    my_blockchain->new_block(my_blockchain->proof_of_work());
}

void App::new_transaction(std::string param)
{
    // std::cout << "debug line 122, param is " << param << " ,param.find(" ") " << param.find(" ") << std::endl;
    std::string* sender = new std::string(param.erase(0, 1)/*for first " "*/, 0, param.find(" "));
    // std::cout << "debug line 122, sender is " << *sender << " ** " << param.find(" ") << std::endl;
    param.assign(param, param.find(" "), std::string::npos);//bug for param.assign(param.find(" "), std::string::npos) ?
    // std::cout << "debug line 125, param is " << param << "** " << param.size() << "** " << param[0] << param [1] << "** " << param.find(" ") << std::endl;
    std::string* recipient = new std::string(param.erase(0, 1)/*for first " "*/, 0, param.find(" "));//bug for std::string recipient(param.erase(0, 1), 0, param.find(" ")); ?
    // std::cout << "debug line 127, recipient is " << *recipient << std::endl;
    param.assign(param, param.find(" "), std::string::npos);
    // std::cout << "debug line 128, param is " << param << std::endl;
    int mount = atoi(param.c_str());
    // std::cout << "debug line 130, mount is " << mount << std::endl;
    
    Transaction* new_transaction = new Transaction(*sender, *recipient, mount);
    my_blockchain->new_transaction(new_transaction);
}

void App::full_chain(std::string response_ip_port)
{
    std::stringstream ss;
    auto str_vector = my_blockchain->get_chain();
    for(auto it = str_vector.begin(); it != str_vector.end(); it++)
    {
        std::cout << *it << std::endl;
        ss << *it << std::endl;
    }
    send_message(response_ip, response_port, ss.str().c_str(), "", "");
}

//TODO : add more nodes for once
void App::register_nodes(std::string param)
{
    my_blockchain->register_node(new Node(param));
}

void App::consensus()
{
    my_blockchain->resolve_conflicts();
}

std::string App::parse_request(std::string str)
{
    return str.assign(str, 0, str.find(" "));
}

std::string App::parse_param(std::string str)
{
    return str.find(" ") == std::string::npos ? "" : str.assign(str, str.find(" "), std::string::npos);
}
