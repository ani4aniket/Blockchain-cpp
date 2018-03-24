#include "Blockchain.h"

Transaction::Transaction()
{

}

Transaction::Transaction(std::string sender, std::string recipient, int mount)
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
    os << std::string("sender: ") << transaction.sender << std::string(" recipient: ") << transaction.recipient << std::string(" mount: ")/* << transaction.mount*/;
    return os;
}