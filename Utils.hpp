#ifndef UTILS_HPP
#define UTILS_HPP

#include "ircserv.hpp"

/**
 * @brief Utility functions for the IRC server
 * 
 * This class contains static helper functions that can be used throughout the program.
 * Static functions belong to the class itself, not to any instance of the class.
 * You can call them without creating an object: Utils::functionName()
 */
class Utils {
public:
    // String manipulation
    static std::vector<std::string> split(const std::string& str, char delimiter);
    static std::string trim(const std::string& str);
    static std::string toUpper(const std::string& str);
    static std::string toLower(const std::string& str);
    
    // Network utilities
    static bool sendToClient(Client* client, const std::string& message);
    static std::string getTimestamp();
    
    // Validation functions
    static bool isValidNickname(const std::string& nickname);
    static bool isValidChannelName(const std::string& channelName);
    
    // IRC formatting
    static std::string formatMessage(const std::string& prefix, const std::string& command, 
                                   const std::string& params);
    static std::string formatReply(int code, const std::string& target, const std::string& message);
    static std::string formatReply(const std::string& serverName, int code, const std::string& target, const std::string& message);
    
    // Number conversion with error checking
    static bool stringToInt(const std::string& str, int& result);
    static std::string intToString(int value);
};

/**
 * @brief IRC namespace containing all IRC protocol constants
 * 
 * This namespace organizes all IRC numeric reply codes as defined in RFC 1459.
 * Using a namespace provides better code organization and prevents global namespace pollution.
 */
namespace IRC {
    // Success reply codes (001-099)
    const int RPL_WELCOME = 001;
    const int RPL_YOURHOST = 002;
    const int RPL_CREATED = 003;
    const int RPL_MYINFO = 004;
    
    // Command response codes (300-399)
    const int RPL_TOPIC = 332;
    const int RPL_NAMREPLY = 353;
    const int RPL_ENDOFNAMES = 366;
    const int RPL_CHANNELMODEIS = 324;
    
    // Error codes (400-599)
    const int ERR_NOSUCHNICK = 401;
    const int ERR_NOSUCHCHANNEL = 403;
    const int ERR_CANNOTSENDTOCHAN = 404;
    const int ERR_NORECIPIENT = 411;
    const int ERR_NOTEXTTOSEND = 412;
    const int ERR_UNKNOWNCOMMAND = 421;
    const int ERR_NONICKNAMEGIVEN = 431;
    const int ERR_ERRONEUSNICKNAME = 432;
    const int ERR_NICKNAMEINUSE = 433;
    const int ERR_USERNOTINCHANNEL = 441;
    const int ERR_NOTONCHANNEL = 442;
    const int ERR_USERONCHANNEL = 443;
    const int ERR_NOTREGISTERED = 451;
    const int ERR_NEEDMOREPARAMS = 461;
    const int ERR_ALREADYREGISTERED = 462;
    const int ERR_PASSWDMISMATCH = 464;
    const int ERR_CHANNELISFULL = 471;
    const int ERR_INVITEONLYCHAN = 473;
    const int ERR_BADCHANNELKEY = 475;
    const int ERR_CHANOPRIVSNEEDED = 482;
}

#endif
