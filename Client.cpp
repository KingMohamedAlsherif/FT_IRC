#include "Client.hpp"

/**
 * @brief Constructor for Client class
 * @param fd File descriptor of the client's socket
 * @param hostname The hostname/IP address of the client
 * 
 * A constructor is a special function that runs when you create a new object.
 * It initializes all the member variables to their starting values.
 */
Client::Client(int fd, const std::string& hostname) 
    : _fd(fd), _hostname(hostname), _authenticated(false), _registered(false), _welcomeSent(false) {
    // The : syntax is called "member initializer list"
    // It's more efficient than setting variables inside the constructor body
}

/**
 * @brief Destructor for Client class
 * 
 * A destructor is called when an object is destroyed.
 * The ~ symbol indicates it's a destructor.
 * It cleans up resources that the object was using.
 */
Client::~Client() {
    // In this case, we don't need to do anything special
    // The socket will be closed by the Server class
}

/**
 * @brief Get the file descriptor
 * @return The socket file descriptor
 * 
 * A getter function returns the value of a private member.
 * 'const' at the end means this function won't modify the object.
 */
int Client::getFd() const {
    return _fd;
}

/**
 * @brief Get the client's nickname
 * @return Reference to the nickname string
 * 
 * Returning a const reference (&) is efficient because:
 * 1. We don't copy the string (just return a reference to it)
 * 2. const prevents the caller from modifying our internal data
 */
const std::string& Client::getNickname() const {
    return _nickname;
}

/**
 * @brief Get the client's username
 * @return Reference to the username string
 */
const std::string& Client::getUsername() const {
    return _username;
}

/**
 * @brief Get the client's real name
 * @return Reference to the realname string
 */
const std::string& Client::getRealname() const {
    return _realname;
}

/**
 * @brief Get the client's hostname
 * @return Reference to the hostname string
 */
const std::string& Client::getHostname() const {
    return _hostname;
}

/**
 * @brief Get the client's input buffer
 * @return Reference to the buffer string
 */
const std::string& Client::getBuffer() const {
    return _buffer;
}

/**
 * @brief Check if client is authenticated
 * @return true if authenticated, false otherwise
 */
bool Client::isAuthenticated() const {
    return _authenticated;
}

/**
 * @brief Check if client is registered
 * @return true if registered, false otherwise
 */
bool Client::isRegistered() const {
    return _registered;
}

/**
 * @brief Check if welcome message was sent
 * @return true if welcome sent, false otherwise
 */
bool Client::isWelcomeSent() const {
    return _welcomeSent;
}

/**
 * @brief Set the client's nickname
 * @param nickname The new nickname
 * 
 * A setter function modifies a private member variable.
 * We take the parameter by const reference to avoid copying.
 */
void Client::setNickname(const std::string& nickname) {
    _nickname = nickname;
}

/**
 * @brief Set the client's username
 * @param username The new username
 */
void Client::setUsername(const std::string& username) {
    _username = username;
}

/**
 * @brief Set the client's real name
 * @param realname The new real name
 */
void Client::setRealname(const std::string& realname) {
    _realname = realname;
}

/**
 * @brief Set authentication status
 * @param auth Authentication status
 */
void Client::setAuthenticated(bool auth) {
    _authenticated = auth;
}

/**
 * @brief Set registration status
 * @param reg Registration status
 */
void Client::setRegistered(bool reg) {
    _registered = reg;
}

/**
 * @brief Set welcome message sent status
 * @param sent Welcome sent status
 */
void Client::setWelcomeSent(bool sent) {
    _welcomeSent = sent;
}

/**
 * @brief Add data to the client's input buffer
 * @param data The data to add
 * 
 * The buffer stores incoming data until we have a complete command.
 * IRC commands end with \r\n (carriage return + line feed).
 */
void Client::appendToBuffer(const std::string& data) {
    _buffer += data;
}

/**
 * @brief Clear the client's input buffer
 */
void Client::clearBuffer() {
    _buffer.clear();  // clear() is a std::string method that empties the string
}

/**
 * @brief Get the IRC prefix for this client
 * @return The prefix string in format "nickname!username@hostname"
 * 
 * IRC messages from clients are prefixed with their identity.
 * This is used when forwarding messages to other clients.
 */
std::string Client::getPrefix() const {
    return _nickname + "!" + _username + "@" + _hostname;
}
