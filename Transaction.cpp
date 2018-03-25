#include "Blockchain.h"

#include <sstream>

Transaction::Transaction()
{

}

Transaction::Transaction(std::string sender, std::string recipient, int mount)
: sender(sender)
, recipient(recipient)
, mount(mount)
{

}

Transaction::~Transaction()
{
    
}

Transaction::operator std::string() const
{
    return std::string("");
}

std::ostream& operator<<(std::ostream& os, const Transaction& transaction)
{
    //???
    //no match for ‘operator<<’ (operand types are ‘std::basic_ostream<char>’ and ‘const char [13]’)
    std::stringstream sstream;
    sstream << std::string("sender: ") << transaction.sender << std::string(" recipient: ") << transaction.recipient << std::string(" mount: ") << transaction.mount;
    os << sstream.str();
    return os;
}