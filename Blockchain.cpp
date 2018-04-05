#include "Blockchain.h"

#include <functional>
#include <iostream>
#include <sstream>
#include <regex>

void send_message(const std::string& sendto_ip, const std::string& sendto_port, const std::string& message, const std::string& receive_ip, const std::string& receive_port);

std::string receive_message(std::string listen_port);

Blockchain::Blockchain()
{
    new_block(new Proof());
}

Blockchain::Blockchain(Node* node)
:self(*node)
{
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
    for(auto it = ++(chain.cbegin()), last_it = chain.cbegin(); it != chain.cend(); it++, last_it++)
    {
        if((*it)->get_previous_hash() != hash(*last_it))
        {
            return false;
        }

        if(!valid_proof(hash((*it)->get_proof()), (*it)->get_previous_hash()))
        {
            return false;
        }
    }

    return true;
}

//TODO (return replaced): return true: Our chain was replaced, return false: Our chain is authoritative
bool Blockchain::resolve_conflicts()
{
    for(auto it = nodes.begin(); it != nodes.end(); it++)
    {
        send_message((*it)->get_ip(), (*it)->get_port(), "full_chain " + self.get_ip() + std::string(" ") + std::string("6001"), "", "");
        std::string full_chain = receive_message("6001");

        std::list<Block*> blocks;

        std::smatch m_index;
        std::smatch m_timestamp;
        std::smatch m_proof;
        std::smatch m_previous_hash;
        std::smatch m_transaction;
        std::regex index("index : ([0-9]+)");
        std::regex timestamp("timestamp : ([0-9]+)");
        std::regex proof("proof : (.+)");
        std::regex previous_hash("previous_hash : (.+)");
        std::regex transaction("transaction [0-9]+ : ([\\s\\S]+)");

        std::smatch m_chain;
        std::regex reg_block("(index[\\s\\S]+?\\n\\n)");

        while(std::regex_search(full_chain, m_chain, reg_block))
        {
            //std::regex_search(m_chain.str(1), m_index, index);使用了被删除的函数？
            std::string m_chain_str1 = m_chain.str(1);
            std::regex_search(m_chain_str1, m_index, index);
            std::regex_search(m_chain_str1, m_timestamp, timestamp);
            std::regex_search(m_chain_str1, m_proof, proof);
            std::regex_search(m_chain_str1, m_previous_hash, previous_hash);
            std::regex_search(m_chain_str1, m_transaction, transaction);

            // std::cout << "m_index match is: \n" << m_index.str(1) << std::endl;
            // std::cout << "m_timestamp match is: \n" << m_timestamp.str(1) << std::endl;
            // std::cout << "m_proof match is: \n" << m_proof.str(1) << std::endl;
            // std::cout << "m_previous_hash match is: \n" << m_previous_hash.str(1) << std::endl;
            // std::cout << "m_transaction match is: \n" << m_transaction.str(1) << std::endl;

            std::smatch m_sender;
            std::smatch m_recipient;
            std::smatch m_mount;
            std::regex sender("sender: ([\\S]+)");
            std::regex recipient("recipient: ([\\S]+)");
            std::regex mount("mount: ([0-9]+)");

            std::string transactions = m_transaction.str(1);
            std::regex one_line("(sender.+mount.+)");
            std::smatch m_line;

            std::vector<Transaction*>* transaction_vector = new std::vector<Transaction*>();

            while(std::regex_search(transactions, m_line, one_line))
            {
                std::string m_line_str1 = m_line.str(1);
                std::regex_search(m_line_str1, m_sender, sender);
                std::regex_search(m_line_str1, m_recipient, recipient);
                std::regex_search(m_line_str1, m_mount, mount);

                // std::cout << "m_sender match is: \n" << m_sender.str(1) << std::endl;
                // std::cout << "m_recipient match is: \n" << m_recipient.str(1) << std::endl;
                // std::cout << "m_mount match is: \n" << m_mount.str(1) << std::endl;

                transaction_vector->push_back(new Transaction(m_sender.str(1), m_recipient.str(1), stoi(m_mount.str(1))));

                transactions = m_line.suffix().str();
            }

            blocks.push_back(new Block(std::stoull(m_index.str(1)), std::stoull(m_timestamp.str(1)), new Proof(m_proof.str(1)), std::stoull(m_previous_hash.str(1)), *transaction_vector));

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
    chain.push_back(block);
}

int Blockchain::new_transaction(Transaction* transaction)
{
    current_transactions.push_back(transaction);
    return chain.size();
}

size_t Blockchain::hash(const std::string str)
{
    std::hash<std::string> hashfun_str;
    return hashfun_str(str);
}

size_t Blockchain::hash(const size_t sizet)
{
    std::hash<size_t> hashfun_sizet;
    return hashfun_sizet(sizet);
}

size_t Blockchain::hash(const Block* block)
{
    if(!block)
        return 0;

    std::hash<std::string> hashfun_str;
    return hashfun_str(block->string());
}

size_t Blockchain::hash(const Proof* proof)
{
    std::hash<std::string> hashfun_str;
    return hashfun_str(static_cast<std::string>(*proof));
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
    size_t hash3 = hash((hash1 / 2) + (hash2 / 2));
    return (hash3 / 100 * 100) == hash3;
}

std::vector<std::string> Blockchain::get_chain()
{
    std::vector<std::string> str_vector;
    std::stringstream ss;
    for(auto it = chain.begin(); it != chain.end(); it++)//for :in?
    {
        str_vector.push_back((*it)->string());
        ss << (*it)->string();
    }
    return str_vector;
}
