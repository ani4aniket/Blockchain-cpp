#include "Blockchain.h"

#include <functional>

Blockchain::Blockchain()
{
    new_block(new Proof());
}

Blockchain::~Blockchain()
{

}

bool Blockchain::register_node(Node* node)
{
    return (nodes.insert(node)).second;
}

bool Blockchain::valid_chain(std::list<Block*> chain)//static or member?
{
    for(auto it = chain.cbegin()++, last_it = chain.cbegin(); it != chain.cend(); it++, last_it++)
    {
        if((*it)->get_previous_hash() != hash(*last_it))
            return false;

        if(valid_proof(hash((*it)->get_proof()), (*it)->get_previous_hash()))
        {
            return false;
        }
    }

    return true;
}

bool Blockchain::resolve_conflicts()
{
    for(auto it = nodes.begin(); it != nodes.end(); it++)
    {
        if((*it)->get_chain().size() > this->chain.size() && valid_chain((*it)->get_chain()))
        {
            this->chain = (*it)->get_chain();
        } 
    }

    return true;
}

void Blockchain::new_block(Proof* proof)
{
    Block* block = new Block(proof, &current_transactions, hash(last_block()), chain.size() + 1);
    chain.push_back(block);
}

int Blockchain::new_transaction(Transaction* transaction)
{
    current_transactions.push_back(transaction);
    return chain.size();
}

size_t Blockchain::hash(std::string str)
{
    std::hash<std::string> hashfun_str;
    return hashfun_str(str);
}

size_t Blockchain::hash(size_t sizet)
{

}

size_t Blockchain::hash(Block* block)
{

}

size_t Blockchain::hash(Proof* proof)
{

}

Block* Blockchain::last_block()
{
    return *chain.end();
}

Proof* Blockchain::proof_of_work()
{
    Proof* proof = new Proof();
    while(!valid_proof(hash(last_block()), hash(proof)))
    {
        proof->change();
    }

    return proof;
}

bool Blockchain::valid_proof(size_t hash1, size_t hash2)
{
    size_t hash3 = hash(hash1 + hash2);
    return hash3 / 10000 * 10000 == hash3;
}

std::vector<std::string> Blockchain::get_chain()
{
    std::vector<std::string> str_vector;
    for(auto it = chain.begin(); it != chain.end(); it++)//for :in?
    {
        str_vector.push_back((*it)->string());
    }
    return str_vector;
}