#include <vector>
#include <string>
#include <list>
#include <set>

class Node {
    public:
    Node();

    virtual ~Node();

    struct classcomp 
    {
        bool operator() (const Node*& lhs, const Node*& rhs) const
        {
            return lhs->string() < rhs->string();
        }
    };

    std::string string() const;

    private:
    std::string url;
};

class Proof {
    public:
    Proof();

    virtual ~Proof();

    std::string string();

    private:

    std::string proof_str;
};

class Transaction {
    public:
    Transaction();

    virtual ~Transaction();

    private:

    std::string sender;

    std::string recipient;

    int mount;
};

class Block {
    public:
    Block();

    virtual ~Block();

    std::string get_proof();

    std::string get_previous_hash();

    private:

    std::string previous_hash;

    std::vector<Transaction*> transactions;

    std::string timestamp;

    int index;

    Proof proof;
};

class Blockchain {

public:
Blockchain();//init

virtual ~Blockchain();

bool register_node(Node* node);//change parameter to reference?

bool valid_chain();

bool resolve_conflicts();

void new_block(const Proof* proof, const std::vector<Transaction*>* transactions = nullptr);

int new_transaction();

std::string hash();

Block* last_block();

Proof* proof_of_work();

bool valid_proof();

private:
std::vector<Transaction*> current_transactions;

std::list<Block*> chain;

std::set<Node*, Node::classcomp> nodes;

};