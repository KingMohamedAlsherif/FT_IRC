#include "Channel.hpp"
#include "Client.hpp"
#include "Utils.hpp"

/**
 * @brief Constructor for Channel class
 * @param name The name of the channel
 * 
 * std::vector is like a dynamic array that can grow and shrink.
 * We initialize all modes to false and user limit to 0.
 */
Channel::Channel(const std::string& name) 
    : _name(name), _inviteOnly(false), _topicRestricted(false), 
      _hasKey(false), _hasUserLimit(false), _userLimit(0) {
}

/**
 * @brief Destructor for Channel class
 */
Channel::~Channel() {
    // We don't delete the Client pointers because they're owned by the Server
    // We just clear our vectors
    _clients.clear();
    _operators.clear();
    _invited.clear();
}

/**
 * @brief Get the channel name
 * @return Reference to the channel name
 */
const std::string& Channel::getName() const {
    return _name;
}

/**
 * @brief Get the channel topic
 * @return Reference to the channel topic
 */
const std::string& Channel::getTopic() const {
    return _topic;
}

/**
 * @brief Get the channel key (password)
 * @return Reference to the channel key
 */
const std::string& Channel::getKey() const {
    return _key;
}

/**
 * @brief Get the list of clients in the channel
 * @return Reference to the clients vector
 * 
 * std::vector<Client*> means a vector that stores pointers to Client objects.
 * Pointers are memory addresses that point to objects.
 */
const std::vector<Client*>& Channel::getClients() const {
    return _clients;
}

/**
 * @brief Get the list of operators in the channel
 * @return Reference to the operators vector
 */
const std::vector<Client*>& Channel::getOperators() const {
    return _operators;
}

/**
 * @brief Get the user limit
 * @return The maximum number of users allowed
 */
size_t Channel::getUserLimit() const {
    return _userLimit;
}

/**
 * @brief Check if channel is invite-only
 * @return true if invite-only, false otherwise
 */
bool Channel::isInviteOnly() const {
    return _inviteOnly;
}

/**
 * @brief Check if topic is restricted to operators
 * @return true if restricted, false otherwise
 */
bool Channel::isTopicRestricted() const {
    return _topicRestricted;
}

/**
 * @brief Check if channel has a key (password)
 * @return true if has key, false otherwise
 */
bool Channel::hasKey() const {
    return _hasKey;
}

/**
 * @brief Check if channel has a user limit
 * @return true if has limit, false otherwise
 */
bool Channel::hasUserLimit() const {
    return _hasUserLimit;
}

/**
 * @brief Add a client to the channel
 * @param client Pointer to the client to add
 * 
 * We use push_back() to add elements to the end of a vector.
 * We check if the client is already in the channel to avoid duplicates.
 */
void Channel::addClient(Client* client) {
    if (!hasClient(client)) {
        _clients.push_back(client);
        // If this is the first client, make them an operator
        if (_clients.size() == 1) {
            addOperator(client);
        }
    }
}

/**
 * @brief Remove a client from the channel
 * @param client Pointer to the client to remove
 * 
 * std::find searches for an element in a container.
 * erase() removes an element from a vector.
 * We also remove them from operators and invited lists.
 */
void Channel::removeClient(Client* client) {
    std::vector<Client*>::iterator it = std::find(_clients.begin(), _clients.end(), client);
    if (it != _clients.end()) {
        _clients.erase(it);
    }
    
    // Also remove from operators and invited lists
    removeOperator(client);
    removeInvited(client);
}

/**
 * @brief Check if a client is in the channel
 * @param client Pointer to the client to check
 * @return true if client is in channel, false otherwise
 */
bool Channel::hasClient(Client* client) const {
    return std::find(_clients.begin(), _clients.end(), client) != _clients.end();
}

/**
 * @brief Get the number of clients in the channel
 * @return Number of clients
 */
size_t Channel::getClientCount() const {
    return _clients.size();
}

/**
 * @brief Add a client to the operators list
 * @param client Pointer to the client to make an operator
 */
void Channel::addOperator(Client* client) {
    if (!isOperator(client)) {
        _operators.push_back(client);
    }
}

/**
 * @brief Remove a client from the operators list
 * @param client Pointer to the client to remove from operators
 */
void Channel::removeOperator(Client* client) {
    std::vector<Client*>::iterator it = std::find(_operators.begin(), _operators.end(), client);
    if (it != _operators.end()) {
        _operators.erase(it);
    }
}

/**
 * @brief Check if a client is an operator
 * @param client Pointer to the client to check
 * @return true if client is an operator, false otherwise
 */
bool Channel::isOperator(Client* client) const {
    return std::find(_operators.begin(), _operators.end(), client) != _operators.end();
}

/**
 * @brief Add a client to the invited list
 * @param client Pointer to the client to invite
 */
void Channel::addInvited(Client* client) {
    if (!isInvited(client)) {
        _invited.push_back(client);
    }
}

/**
 * @brief Remove a client from the invited list
 * @param client Pointer to the client to remove from invited list
 */
void Channel::removeInvited(Client* client) {
    std::vector<Client*>::iterator it = std::find(_invited.begin(), _invited.end(), client);
    if (it != _invited.end()) {
        _invited.erase(it);
    }
}

/**
 * @brief Check if a client is invited
 * @param client Pointer to the client to check
 * @return true if client is invited, false otherwise
 */
bool Channel::isInvited(Client* client) const {
    return std::find(_invited.begin(), _invited.end(), client) != _invited.end();
}

/**
 * @brief Set the channel topic
 * @param topic The new topic
 */
void Channel::setTopic(const std::string& topic) {
    _topic = topic;
}

/**
 * @brief Set the channel key (password)
 * @param key The new key
 */
void Channel::setKey(const std::string& key) {
    _key = key;
    _hasKey = true;
}

/**
 * @brief Remove the channel key
 */
void Channel::removeKey() {
    _key.clear();
    _hasKey = false;
}

/**
 * @brief Set the user limit
 * @param limit The maximum number of users
 */
void Channel::setUserLimit(size_t limit) {
    _userLimit = limit;
    _hasUserLimit = true;
}

/**
 * @brief Remove the user limit
 */
void Channel::removeUserLimit() {
    _userLimit = 0;
    _hasUserLimit = false;
}

/**
 * @brief Set invite-only mode
 * @param inviteOnly true to enable invite-only, false to disable
 */
void Channel::setInviteOnly(bool inviteOnly) {
    _inviteOnly = inviteOnly;
}

/**
 * @brief Set topic restriction mode
 * @param restricted true to restrict topic changes to operators, false to allow all
 */
void Channel::setTopicRestricted(bool restricted) {
    _topicRestricted = restricted;
}

/**
 * @brief Get the channel modes as a string
 * @return String representation of channel modes (e.g., "+itk")
 */
std::string Channel::getModeString() const {
    std::string modes = "+";
    
    if (_inviteOnly) modes += "i";
    if (_topicRestricted) modes += "t";
    if (_hasKey) modes += "k";
    if (_hasUserLimit) modes += "l";
    
    if (modes == "+") {
        return "";  // No modes set
    }
    
    return modes;
}

/**
 * @brief Get the list of users for the NAMES command
 * @return String with all usernames, operators prefixed with @
 */
std::string Channel::getUserList() const {
    std::string userList;
    
    for (size_t i = 0; i < _clients.size(); ++i) {
        if (i > 0) userList += " ";
        
        // Prefix operators with @
        if (isOperator(_clients[i])) {
            userList += "@";
        }
        
        userList += _clients[i]->getNickname();
    }
    
    return userList;
}

/**
 * @brief Send a message to all clients in the channel
 * @param message The message to send
 * @param exclude Client to exclude from the broadcast (usually the sender)
 * 
 * This function will be implemented after we create the Utils class
 * for sending messages to clients.
 */
void Channel::broadcast(const std::string& message, Client* exclude) {
    for (size_t i = 0; i < _clients.size(); ++i) {
        if (_clients[i] != exclude) {
            // We'll implement sendToClient in Utils.cpp
            Utils::sendToClient(_clients[i], message);
        }
    }
}
