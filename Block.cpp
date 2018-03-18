#include "Blockchain.h"

Block::Block()
{

}

Block::Block(Proof* proof, const std::vector<Transaction*>* transactions, size_t pervious_hash)
{

}

Block::~Block()
{
    
}

Proof* Block::get_proof() const
{

}

size_t Block::get_previous_hash() const
{
    return previous_hash;
}