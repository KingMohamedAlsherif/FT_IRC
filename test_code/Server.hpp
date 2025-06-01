#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <string>
#include <vector>

class Server
{
private:
    int _port;
    std::string _password;
    int _server_fd; // Server socket file descriptor
    struct sockaddr_in _address;
    std::vector<struct pollfd> _poll_fds;     // Vector for poll() file descriptors
    std::vector<std::string> _client_buffers; // Buffers for client messages

public:
    Server(int port, const std::string &password);
    ~Server();
    void start();

private:
    void setupSocket();
    void acceptNewClient();
    void handleClient(int client_fd, int index);
};

#endif