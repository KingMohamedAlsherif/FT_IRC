#include "Utils.hpp"
#include "Client.hpp"
#include <sys/time.h>
#include <climits>     // For INT_MAX and INT_MIN
#include <iomanip>     // For setfill and setw

/**
 * @brief Split a string by a delimiter
 * @param str The string to split
 * @param delimiter The character to split by
 * @return Vector of strings
 * 
 * This function takes a string like "hello world test" and splits it by spaces
 * to return a vector containing ["hello", "world", "test"]
 */
std::vector<std::string> Utils::split(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(str);  // stringstream lets us treat a string like a file
    std::string item;
    
    // getline extracts characters until it finds the delimiter
    while (std::getline(ss, item, delimiter)) {
        if (!item.empty()) {  // Skip empty strings
            result.push_back(item);
        }
    }
    
    return result;
}

/**
 * @brief Remove whitespace from the beginning and end of a string
 * @param str The string to trim
 * @return Trimmed string
 * 
 * Trimming removes spaces, tabs, newlines from the start and end of a string.
 */
std::string Utils::trim(const std::string& str) {
    if (str.empty()) return str;
    
    // Find first non-whitespace character
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";  // String is all whitespace
    
    // Find last non-whitespace character
    size_t end = str.find_last_not_of(" \t\r\n");
    
    // Extract substring from start to end
    return str.substr(start, end - start + 1);
}

/**
 * @brief Convert string to uppercase
 * @param str The string to convert
 * @return Uppercase string
 */
std::string Utils::toUpper(const std::string& str) {
    std::string result = str;
    for (size_t i = 0; i < result.length(); ++i) {
        if (result[i] >= 'a' && result[i] <= 'z') {
            result[i] = result[i] - 'a' + 'A';  // Convert to uppercase
        }
    }
    return result;
}

/**
 * @brief Convert string to lowercase
 * @param str The string to convert
 * @return Lowercase string
 */
std::string Utils::toLower(const std::string& str) {
    std::string result = str;
    for (size_t i = 0; i < result.length(); ++i) {
        if (result[i] >= 'A' && result[i] <= 'Z') {
            result[i] = result[i] - 'A' + 'a';  // Convert to lowercase
        }
    }
    return result;
}

/**
 * @brief Send a message to a client
 * @param client Pointer to the client
 * @param message The message to send
 * @return true if successful, false if failed
 * 
 * send() is a system function that sends data over a socket.
 * It returns the number of bytes sent, or -1 on error.
 */
bool Utils::sendToClient(Client* client, const std::string& message) {
    if (!client) return false;
    
    std::string fullMessage = message + "\r\n";  // IRC messages end with \r\n
    
    // send(socket, data, length, flags)
    // On macOS, MSG_NOSIGNAL is not available. Using 0 for flags.
    // For robust SIGPIPE handling on macOS, SO_NOSIGPIPE socket option should be set elsewhere.
    ssize_t bytesSent = send(client->getFd(), fullMessage.c_str(), fullMessage.length(), 0);
    
    if (bytesSent < 0) {
        std::cerr << "Error sending to client: " << strerror(errno) << std::endl;
        return false;
    }
    
    if (static_cast<size_t>(bytesSent) != fullMessage.length()) {
        std::cerr << "Warning: Partial send to client" << std::endl;
        return false;
    }
    
    return true;
}

/**
 * @brief Get current timestamp as string
 * @return Timestamp string
 */
std::string Utils::getTimestamp() {
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);
    
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    return std::string(buffer);
}

/**
 * @brief Check if a nickname is valid according to IRC rules
 * @param nickname The nickname to validate
 * @return true if valid, false otherwise
 * 
 * IRC nicknames must start with a letter and contain only letters, numbers, and certain symbols.
 */
bool Utils::isValidNickname(const std::string& nickname) {
    if (nickname.empty() || nickname.length() > 30) {
        return false;
    }
    
    // First character must be a letter
    char first = nickname[0];
    if (!((first >= 'a' && first <= 'z') || (first >= 'A' && first <= 'Z'))) {
        return false;
    }
    
    // Rest can be letters, numbers, or allowed symbols
    for (size_t i = 1; i < nickname.length(); ++i) {
        char c = nickname[i];
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
              (c >= '0' && c <= '9') || c == '_' || c == '-' || c == '[' || 
              c == ']' || c == '{' || c == '}' || c == '\\' || c == '`' || c == '|')) {
            return false;
        }
    }
    
    return true;
}

/**
 * @brief Check if a channel name is valid
 * @param channelName The channel name to validate
 * @return true if valid, false otherwise
 * 
 * Channel names must start with # and contain valid characters.
 */
bool Utils::isValidChannelName(const std::string& channelName) {
    if (channelName.empty() || channelName[0] != '#' || channelName.length() < 2) {
        return false;
    }
    
    // Check for invalid characters
    for (size_t i = 1; i < channelName.length(); ++i) {
        char c = channelName[i];
        if (c == ' ' || c == ',' || c == '\r' || c == '\n' || c == '\0') {
            return false;
        }
    }
    
    return true;
}

/**
 * @brief Format an IRC message
 * @param prefix The message prefix (sender)
 * @param command The IRC command
 * @param params The command parameters
 * @return Formatted IRC message
 * 
 * IRC messages have the format: :prefix COMMAND params
 */
std::string Utils::formatMessage(const std::string& prefix, const std::string& command, 
                                const std::string& params) {
    std::string message;
    
    if (!prefix.empty()) {
        message += ":" + prefix + " ";
    }
    
    message += command;
    
    if (!params.empty()) {
        message += " " + params;
    }
    
    return message;
}

/**
 * @brief Format a numeric reply
 * @param code The numeric code (e.g., 001 for welcome)
 * @param target The target (usually the client's nickname)
 * @param message The reply message
 * @return Formatted numeric reply
 */
std::string Utils::formatReply(int code, const std::string& target, const std::string& message) {
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(3) << code;  // Format as 3-digit number with leading zeros
    
    return ss.str() + " " + target + " " + message;
}

/**
 * @brief Format a numeric reply with server prefix (RFC compliant)
 * @param serverName The server name to use as prefix
 * @param code The numeric code (e.g., 001 for welcome)
 * @param target The target (usually the client's nickname)
 * @param message The reply message
 * @return Formatted numeric reply with server prefix
 */
std::string Utils::formatReply(const std::string& serverName, int code, const std::string& target, const std::string& message) {
    std::stringstream ss;
    ss << ":" << serverName << " " << std::setfill('0') << std::setw(3) << code << " " << target << " " << message;
    
    return ss.str();
}

/**
 * @brief Convert string to integer with error checking
 * @param str The string to convert
 * @param result Reference to store the result
 * @return true if conversion successful, false otherwise
 * 
 * This is safer than using atoi() because it checks for errors.
 */
bool Utils::stringToInt(const std::string& str, int& result) {
    if (str.empty()) return false;
    
    char* endPtr;
    long value = strtol(str.c_str(), &endPtr, 10);
    
    // Check if conversion was successful and no extra characters
    if (*endPtr != '\0') return false;
    
    // Check for overflow
    if (value > INT_MAX || value < INT_MIN) return false;
    
    result = static_cast<int>(value);
    return true;
}

/**
 * @brief Convert integer to string
 * @param value The integer to convert
 * @return String representation
 */
std::string Utils::intToString(int value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}
