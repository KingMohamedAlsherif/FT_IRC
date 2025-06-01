#include "Server.hpp"
#include <cstdlib>
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }

    int port = std::atoi(argv[1]);
    if (port < 1024 || port > 65535)
    {
        std::cerr << "Error: Port must be between 1024 and 65535" << std::endl;
        return 1;
    }

    std::string password = argv[2];
    if (password.empty())
    {
        std::cerr << "Error: Password cannot be empty" << std::endl;
        return 1;
    }

    Server server(port, password);
    server.start();
    return 0;
}