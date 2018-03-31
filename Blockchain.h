#include <vector>
#include <string>
#include <list>
#include <set>

#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

class Proof {

    friend std::ostream& operator<<(std::ostream& os, const Proof& proof);

    public:
    Proof();

    Proof(std::string proof_str);

    virtual ~Proof();

    std::string string();

    operator std::string() const;

    void change();

    private:

    std::string proof_str;

    long long proof_ll;
};

class Transaction {

    friend std::ostream& operator<<(std::ostream& os, const Transaction& transaction); 

    public:

    Transaction();

    Transaction(std::string sender, std::string recipient, int mount);

    virtual ~Transaction();

    operator std::string() const;

    private:

    std::string sender;

    std::string recipient;

    int mount;
};

class Block {
    public:
    Block();

    Block(Proof* proof, const std::vector<Transaction*>* transactions, size_t previous_hash, int index);//const ?

    Block(int index, int timestamp, Proof* proof, int previous_hash, std::vector<Transaction*> transactions);

    virtual ~Block();

    Proof* get_proof() const;

    size_t get_previous_hash() const;

    std::string string() const;

    private:

    int index;

    long long timestamp;

    Proof proof;

    size_t previous_hash;

    std::vector<Transaction*> transactions;
};

class Node {
    public:
    Node();

    Node(std::string ip_port);

    Node(std::string ip, std::string port);

    virtual ~Node();

    std::list<Block*>& get_chain() const;

    struct classcomp 
    {
        bool operator() (const Node* const & lhs, const Node* const & rhs) const
        {
            return lhs->string() < rhs->string();
        }
    };

    std::string string() const;

    std::string get_ip() const;

    std::string get_port() const;

    private:
    std::string ip_port;

    std::string ip;

    std::string port;
};

class Blockchain {

public:
Blockchain();

Blockchain(Node* node);

virtual ~Blockchain();

bool register_node(Node* node);//change parameter to reference?

bool valid_chain(std::list<Block*> chain);

bool resolve_conflicts();

void new_block(Proof* proof);

int new_transaction(Transaction* transaction);

size_t hash(std::string str);

size_t hash(size_t sizet);

size_t hash(Block* block);

size_t hash(Proof* proof);

Block* last_block();

Proof* proof_of_work();

bool valid_proof(size_t proof, size_t previous_hash);

std::vector<std::string> get_chain();

private:
std::vector<Transaction*> current_transactions;

std::list<Block*> chain;

std::set<Node*, Node::classcomp> nodes;

Node self;

};

#endif // !BLOCKCHAIN_H