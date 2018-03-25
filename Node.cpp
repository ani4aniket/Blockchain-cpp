#include "Blockchain.h"

Node::Node()
: ip_port("")
{

}

Node::Node(std::string ip_port)
: ip_port(ip_port)
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
