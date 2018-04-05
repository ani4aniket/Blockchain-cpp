#include "Blockchain.h"

#include <iostream>

Node::Node()
: ip_port("")
{

}

Node::Node(std::string ip_port)
: ip_port(ip_port)
{
    ip = std::string(ip_port, 1, ip_port.find(":") - 1);//1 for " ", - 1 for ":"
    port = std::string(ip_port, ip_port.find(":") + 1, std::string::npos);
}

Node::Node(std::string ip, std::string port)
: ip(ip)
, port(port)
{

}

Node::~Node()
{

}

std::list<Block*>& Node::get_chain() const
{

}

std::string Node::string() const
{
    return ip_port;
}

std::string Node::get_ip() const
{
    return ip;
}

std::string Node::get_port() const
{
    return port;
}
