#include "App.h"

int main(int argc, char** argv)
{
    (new App("127.0.0.1", argv[1]))->run(argv[1]);

    return 0;
}

int argv_to_port(char** argv)
{
    
}