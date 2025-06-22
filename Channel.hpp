#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "ircserv.hpp"

/**
 * @brief The Channel class represents an IRC channel
 * 
 * A channel is like a chat room where multiple clients can talk.
 * Channels have names (starting with #), topics, modes, and member lists.
 */
class Channel {
private:
    std::string _name;                      // Channel name (e.g., "#general")
    std::string _topic;                     // Channel topic
    std::string _key;                       // Channel password (if any)
    std::vector<Client*> _clients;          // List of clients in the channel
    std::vector<Client*> _operators;        // List of channel operators
    std::vector<Client*> _invited;          // List of invited clients (for invite-only mode)
    
    // Channel modes
    bool _inviteOnly;                       // +i mode: only invited users can join
    bool _topicRestricted;                  // +t mode: only operators can change topic
    bool _hasKey;                           // +k mode: channel has a password
    bool _hasUserLimit;                     // +l mode: channel has user limit
    size_t _userLimit;                      // Maximum number of users

public:
    // Constructor
    Channel(const std::string& name);
    
    // Destructor
    ~Channel();
    
    // Getters
    const std::string& getName() const;
    const std::string& getTopic() const;
    const std::string& getKey() const;
    const std::vector<Client*>& getClients() const;
    const std::vector<Client*>& getOperators() const;
    size_t getUserLimit() const;
    
    // Mode getters
    bool isInviteOnly() const;
    bool isTopicRestricted() const;
    bool hasKey() const;
    bool hasUserLimit() const;
    
    // Client management
    void addClient(Client* client);
    void removeClient(Client* client);
    bool hasClient(Client* client) const;
    size_t getClientCount() const;
    
    // Operator management
    void addOperator(Client* client);
    void removeOperator(Client* client);
    bool isOperator(Client* client) const;
    
    // Invite management
    void addInvited(Client* client);
    void removeInvited(Client* client);
    bool isInvited(Client* client) const;
    
    // Channel operations
    void setTopic(const std::string& topic);
    void setKey(const std::string& key);
    void removeKey();
    void setUserLimit(size_t limit);
    void removeUserLimit();
    void setInviteOnly(bool inviteOnly);
    void setTopicRestricted(bool restricted);
    
    // Utility functions
    std::string getModeString() const;      // Returns the channel modes as a string
    std::string getUserList() const;        // Returns list of users for NAMES command
    void broadcast(const std::string& message, Client* exclude = NULL);  // Send message to all clients
};

#endif
