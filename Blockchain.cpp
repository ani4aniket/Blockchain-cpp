#include "Blockchain.h"

#include <functional>
#include <iostream>
#include <sstream>
#include <regex>

void send_message(const std::string& sendto_ip, const std::string& sendto_port, const std::string& message, const std::string& receive_ip, const std::string& receive_port);

std::string receive_message(std::string listen_port);

Blockchain::Blockchain()
{
    std::cout << "debug l14 " << this << std::endl;
    new_block(new Proof());
}

Blockchain::Blockchain(Node* node)
:self(*node)
{
    std::cout << "debug l21 " << this << std::endl;
    //Blockchain(); why?
    new_block(new Proof());
}

Blockchain::~Blockchain()
{

}

bool Blockchain::register_node(Node* node)
{
    for(auto it = nodes.begin(); it != nodes.end(); it++)
        std::cout << "node: " << (*it)->string() << std::endl;

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

//TODO (return replaced): return true: Our chain was replaced, return false: Our chain is authoritative
bool Blockchain::resolve_conflicts()
{
    std::cout << "debug L58" << std::endl;
    for(auto it = nodes.begin(); it != nodes.end(); it++)
    {
        std::cout << "debug L61" << std::endl;
        send_message((*it)->get_ip(), (*it)->get_port(), "full_chain " + self.get_ip() + std::string(" ") + std::string("6001"), "", "");
        std::cout << "debug L63" << std::endl;
        std::string full_chain = receive_message("6001");

        // std::cout << "debug l63 " << full_chain << std::endl;

        std::list<Block*> blocks;

        std::smatch m_index;
        std::smatch m_timestamp;
        std::smatch m_proof;
        std::smatch m_previous_hash;
        std::smatch m_transaction;
        std::regex index("index : ([0-9]+) ");
        std::regex timestamp("timestamp : ([0-9]+) ");
        std::regex proof("proof : (.+) ");
        std::regex previous_hash("previous_hash : (.+) ");
        std::regex transaction("transaction : (.+)$");

        std::smatch m_chain;
        std::regex reg_chain("(index.+)(index | $)");

        std::regex_search(full_chain, m_chain, reg_chain);

        while(std::regex_search(full_chain, m_chain, reg_chain))
        {
            //std::regex_search(m_chain.str(1), m_index, index);使用了被删除的函数？
            std::string m_chain_str1 = m_chain.str(1);
            std::regex_search(m_chain_str1, m_index, index);
            std::regex_search(m_chain_str1, m_timestamp, timestamp);
            std::regex_search(m_chain_str1, m_proof, proof);
            std::regex_search(m_chain_str1, m_previous_hash, previous_hash);
            std::regex_search(m_chain_str1, m_transaction, transaction);

            std::smatch m_sender;
            std::smatch m_recipient;
            std::smatch m_mount;
            std::regex sender("sender : (.+) ");
            std::regex recipient("recipient : (.+) ");
            std::regex mount("mount : ([0-9]+) ");

            std::string transactions = m_transaction.str(1);
            std::regex one_line("(sender.+mount)");
            std::smatch m_line;

            std::vector<Transaction*>* transaction_vector = new std::vector<Transaction*>();

            while(std::regex_search(transactions, m_line, one_line))
            {
                std::string m_line_str1 = m_line.str(1);
                std::regex_search(m_line_str1, m_sender, sender);
                std::regex_search(m_line_str1, m_recipient, recipient);
                std::regex_search(m_line_str1, m_mount, mount);

                transaction_vector->push_back(new Transaction(m_sender.str(1), m_recipient.str(1), stoi(m_mount.str(1))));

                transactions = m_line.suffix().str();
            }

            blocks.push_back(new Block(stoi(m_index.str(1)), stoi(m_timestamp.str(1)), new Proof(m_proof.str(1)), stoi(m_previous_hash.str(1)), *transaction_vector));

            full_chain = m_chain.suffix().str();
        }

        if(blocks.size() > this->chain.size() && valid_chain(blocks))
        {
            this->chain = blocks;
        } 
    }

    return true;
}

void Blockchain::new_block(Proof* proof)
{
    Block* block = new Block(proof, &current_transactions, hash(last_block()), chain.size() + 1);
    current_transactions.clear();
    std::cout << "debug l133: " << chain.size() << " " << this << std::endl;
    chain.push_back(block);
    std::cout << "debug l135: " << chain.size() << " " << this << std::endl;
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
    if(!block)
        return 0;

    std::hash<std::string> hashfun_str;
    return hashfun_str(block->string());
}

size_t Blockchain::hash(Proof* proof)
{
    std::hash<std::string> hashfun_str;
    return hashfun_str(*proof);
}

Block* Blockchain::last_block()
{
    if(chain.size() == 0)
        return nullptr;

    return chain.back();
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
    // std::cout << "debug line 109, hash3 is " << hash3 << " ** " << hash3 / 100 << std::endl;
    return hash3 / 100 * 100 == hash3;
}

std::vector<std::string> Blockchain::get_chain()
{
    std::cout << "debug l196: " << chain.size() << " " << this << std::endl;
    std::vector<std::string> str_vector;
    std::stringstream ss;
    for(auto it = chain.begin(); it != chain.end(); it++)//for :in?
    {
        str_vector.push_back((*it)->string());
        ss << (*it)->string();
    }
    return str_vector;
}
