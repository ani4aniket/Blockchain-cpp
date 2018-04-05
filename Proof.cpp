#include "Blockchain.h"

#include <sstream>
#include <stdlib.h>
#include <iostream>

Proof::Proof()
: proof_str("0")
, proof_ll(0)
{

}

Proof::Proof(std::string proof_str)
: proof_str(proof_str)
{
    proof_ll = std::stoull(proof_str);
}

Proof::~Proof()
{

}

std::string Proof::string()
{
    return proof_str;
}

Proof::operator std::string() const
{
    return proof_str;
}

void Proof::change()
{
    std::stringstream sstream;
    sstream << proof_ll++;
    proof_str = sstream.str();
    std::cout << "current proof is " << proof_str << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Proof& proof)
{
    std::stringstream ss;
    ss << proof.proof_str;
    os << ss.str();
    return os;
}
