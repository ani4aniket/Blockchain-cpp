//#include <string.h> the difference betwenn <string> and <string.h>

#include "Blockchain.h"

class App {
public:
App();

App(std::string ip, std::string port);

virtual ~App();

void run(std::string listen_port);

private:

void mine(std::string param);

void new_transaction(std::string param);

void full_chain(std::string param); 

void register_nodes(std::string param);

void consensus();

std::string parse_request(std::string str);

std::string parse_param(std::string str);

Blockchain* my_blockchain;
};
