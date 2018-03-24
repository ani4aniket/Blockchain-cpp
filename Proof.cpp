#include "Blockchain.h"

#include <sstream>

Proof::Proof()
: proof_str("null")
{

}

Proof::~Proof()
{

}

std::string Proof::string()
{

}

Proof::operator std::string() const
{
    return std::string("");
}

void Proof::change()
{
    
}

std::ostream& operator<<(std::ostream& os, const Proof& proof)
{
    std::stringstream ss;
    ss << " " << proof.proof_str;
    os << ss.str();
    return os;
}