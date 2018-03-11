#include "Blockchain.h"

#include <functional>

Blockchain::Blockchain()
{
    new_block(nullptr, nullptr);
}

Blockchain::~Blockchain()
{

}

bool Blockchain::register_node(Node* node)
{
    return (nodes.insert(node)).second;
}

bool Blockchain::valid_chain()//static or member?
{
    for(auto it = chain.crbegin(); it != chain.crend(); it--)
    {
        if(valid_proof((*it)->get_proof(), (*it)->get_previous_hash()))
        {
            return false;
        }
    }

    return true;
}

bool Blockchain::resolve_conflicts()
{
    for(std::vector<Node*>:iterator it = nodes.begin; it != nodes.end; it++)
    {
        //
        if(*it->chian.length > this->chain.length && *it->valid_chain())
        {
            this->chain = *it->chian;
        } 
    }

    return true;
}

void Blockchain::new_block(Proof* proof, std::vector<Transaction*>& transactions)
{
    Block* block = new Block(proof, transactions);
    chain.push_back(block);
}

int Blockchain::new_transaction(Transaction* transaction)
{
    current_transactions.push_back(transaction);
    return chain.size();
}

std::string Blockchain::hash(std::string str)
{
    std::hash<std::string> hashfun_str;
    return hashfun_str(str);
}

Block* Blockchain::last_block()
{
    return *chain.end;
}

Proof* Blockchain::proof_of_work()
{
    Proof* proof = new Proof();
    while(!valid_proof(last_block(), proof))
    {
        proof->change();
    }

    return proof;
}

bool Blockchain::valid_proof(std::string str1, std::string str2)
{
    std::string hash_str = hash(str1 + str2);
    return hash_str.find("0000") == hash_str.begin();
}