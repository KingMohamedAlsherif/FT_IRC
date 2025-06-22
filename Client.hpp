#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "ircserv.hpp"

/**
 * @brief The Client class represents a connected IRC client
 * 
 * This class stores all information about a client connected to our IRC server.
 * Each client has a socket file descriptor, nickname, username, and various states.
 */
class Client {
private:
    int _fd;                    // File descriptor for the client's socket connection
    std::string _nickname;      // Client's nickname (what others see)
    std::string _username;      // Client's username (for identification)
    std::string _realname;      // Client's real name
    std::string _hostname;      // Client's hostname/IP address
    std::string _buffer;        // Buffer to store incoming data
    bool _authenticated;        // Whether client has provided correct password
    bool _registered;           // Whether client has completed registration (NICK + USER)
    bool _welcomeSent;          // Whether we've sent the welcome message

public:
    // Constructor
    Client(int fd, const std::string& hostname);
    
    // Destructor
    ~Client();
    
    // Getters (const means they don't modify the object)
    int getFd() const;
    const std::string& getNickname() const;
    const std::string& getUsername() const;
    const std::string& getRealname() const;
    const std::string& getHostname() const;
    const std::string& getBuffer() const;
    bool isAuthenticated() const;
    bool isRegistered() const;
    bool isWelcomeSent() const;
    
    // Setters
    void setNickname(const std::string& nickname);
    void setUsername(const std::string& username);
    void setRealname(const std::string& realname);
    void setAuthenticated(bool auth);
    void setRegistered(bool reg);
    void setWelcomeSent(bool sent);
    
    // Buffer operations
    void appendToBuffer(const std::string& data);
    void clearBuffer();
    
    // Helper functions
    std::string getPrefix() const;  // Returns the IRC prefix (nickname!username@hostname)
};

#endif
