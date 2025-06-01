#include "Server.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>

Server::Server(int port, const std::string &password) : _port(port), _password(password), _server_fd(-1) {}

Server::~Server()
{
    if (_server_fd != -1)
        close(_server_fd);
    for (size_t i = 0; i < _poll_fds.size(); ++i)
        if (_poll_fds[i].fd != -1)
            close(_poll_fds[i].fd);
}

void Server::setupSocket()
{
    // Create socket
    _server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_server_fd == -1)
    {
        std::cerr << "Error: Cannot create socket" << std::endl;
        exit(1);
    }

    // Set socket to non-blocking
    if (fcntl(_server_fd, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cerr << "Error: Cannot set socket to non-blocking" << std::endl;
        exit(1);
    }

    // Allow port reuse
    int opt = 1;
    if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        std::cerr << "Error: Cannot set socket options" << std::endl;
        exit(1);
    }

    // Set up address structure
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons(_port);
    memset(_address.sin_zero, 0, sizeof(_address.sin_zero));

    // Bind socket
    if (bind(_server_fd, (struct sockaddr *)&_address, sizeof(_address)) == -1)
    {
        std::cerr << "Error: Cannot bind socket" << std::endl;
        exit(1);
    }

    // Listen for connections
    if (listen(_server_fd, 10) == -1)
    {
        std::cerr << "Error: Cannot listen on socket" << std::endl;
        exit(1);
    }

    // Add server socket to poll_fds
    struct pollfd server_poll_fd;
    server_poll_fd.fd = _server_fd;
    server_poll_fd.events = POLLIN;
    _poll_fds.push_back(server_poll_fd);
    _client_buffers.push_back(""); // Placeholder for server
}

void Server::acceptNewClient()
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(_server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd == -1)
    {
        std::cerr << "Error: Cannot accept client" << std::endl;
        return;
    }

    // Set client socket to non-blocking
    if (fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cerr << "Error: Cannot set client socket to non-blocking" << std::endl;
        close(client_fd);
        return;
    }

    // Add client to poll_fds
    struct pollfd client_poll_fd;
    client_poll_fd.fd = client_fd;
    client_poll_fd.events = POLLIN;
    _poll_fds.push_back(client_poll_fd);
    _client_buffers.push_back("");
    std::cout << "New client connected: " << client_fd << std::endl;
}

void Server::handleClient(int client_fd, int index)
{
    char buffer[1024];
    int bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0)
    {
        // Client disconnected or error
        std::cout << "Client disconnected: " << client_fd << std::endl;
        close(client_fd);
        _poll_fds.erase(_poll_fds.begin() + index);
        _client_buffers.erase(_client_buffers.begin() + index);
        return;
    }

    buffer[bytes_received] = '\0';
    _client_buffers[index] += buffer;
    std::cout << "Received from " << client_fd << ": " << buffer << std::endl;

    // Echo back to client (simplified, no IRC protocol yet)
    std::string response = "Server: " + _client_buffers[index];
    send(client_fd, response.c_str(), response.length(), 0);
    _client_buffers[index].clear(); // Clear buffer after processing
}

void Server::start()
{
    setupSocket();
    std::cout << "Server listening on port " << _port << std::endl;

    while (true)
    {
        // Poll for events
        int poll_count = poll(&_poll_fds[0], _poll_fds.size(), -1);
        if (poll_count == -1)
        {
            std::cerr << "Error: Poll failed" << std::endl;
            exit(1);
        }

        // Check all file descriptors
        for (size_t i = 0; i < _poll_fds.size(); ++i)
        {
            if (_poll_fds[i].revents & POLLIN)
            {
                if (_poll_fds[i].fd == _server_fd)
                {
                    // New connection
                    acceptNewClient();
                }
                else
                {
                    // Client data
                    handleClient(_poll_fds[i].fd, i);
                }
            }
        }
    }
}