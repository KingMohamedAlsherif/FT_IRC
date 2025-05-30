ft_irc - Internet Relay Chat Server
Overview
The ft_irc project is a C++ 98 implementation of an IRC (Internet Relay Chat) server named ircserv. It enables real-time, text-based communication, allowing multiple clients to connect, authenticate, set nicknames and usernames, join channels, send/receive private messages, and manage channels with operator commands. The server uses non-blocking I/O with a single poll() call for efficient handling of concurrent connections over TCP/IP, adhering to strict project guidelines.
Features

Multi-client Support: Handles multiple simultaneous clients using non-blocking I/O.
TCP/IP Communication: Supports IPv4 (or IPv6) for reliable data transfer.
IRC Protocol Support:
Authentication: Verifies clients using a server password.
User Management: Supports NICK and USER commands to set nicknames and usernames.
Channel Management: Clients can join channels using JOIN and send messages with PRIVMSG.
Private Messaging: Supports direct messages between clients.
Channel Broadcasting: Messages sent to a channel are forwarded to all members.
Operators and Users: Distinguishes between regular users and channel operators.
Operator Commands:
KICK: Ejects a client from a channel.
INVITE: Invites a client to a channel.
TOPIC: Sets or views the channel topic.
MODE: Manages channel modes (i: invite-only, t: topic restriction, k: channel key, o: operator privilege, l: user limit).




Non-blocking I/O: Uses a single poll() call to manage all socket operations (server and clients).
Error Handling: Robust against crashes, partial data, low bandwidth, and disconnections.
C++ 98 Compliance: Uses only C++ 98 standard and allowed socket functions (socket, bind, listen, accept, recv, send, fcntl, poll, etc.).
No External Libraries: Pure C++ 98 implementation, no Boost or other libraries.

Requirements

Compiler: C++ compiler supporting C++ 98 (e.g., g++, clang++).
Operating System: Unix-based system (Linux, macOS).
Dependencies: Standard C++ 98 libraries and POSIX socket libraries (no external libraries).
Testing Tools: An IRC client (e.g., HexChat, mIRC) for full testing, or netcat (nc) for basic connectivity tests.
Reference Client: HexChat (used as the reference client for evaluation; ensure compatibility).

Project Structure
ft_irc/
├── Makefile          # Compiles the project
├── main.cpp          # Entry point, parses arguments, starts server
├── Server.hpp        # Server class declaration
├── Server.cpp        # Server implementation (socket setup, client/channel handling)
├── Client.hpp        # Client class for managing user data
├── Client.cpp        # Client implementation (authentication, message processing)
├── Channel.hpp       # Channel class for managing channels
├── Channel.cpp       # Channel implementation (members, modes, operator commands)
└── README.md         # This file

Compilation

Clone the repository or navigate to the project directory:
git clone <repository_url> ft_irc
cd ft_irc


Compile using the provided Makefile:
make

This generates the ircserv executable. The Makefile includes:

all: Builds the executable.
clean: Removes object files.
fclean: Removes object files and the executable.
re: Rebuilds the project.

Compilation uses flags: -Wall -Wextra -Werror -std=c++98.


Usage
Run the server with a port number and password:
./ircserv <port> <password>


<port>: A port number between 1024 and 65535 (e.g., 6667, a common IRC port).
<password>: A non-empty string required for client authentication.

Example:
./ircserv 6667 mypassword

The server will start and listen on the specified port, displaying:
Server listening on port 6667

Testing
With an IRC Client (Recommended)

Install an IRC client like HexChat or mIRC.
Configure the client to connect to:
Server: localhost or 127.0.0.1
Port: 6667 (or the specified port)
Password: mypassword (or the specified password)


Test the following features:
Set a nickname: NICK testuser
Set a username: USER testuser 0 * :Test User
Join a channel: JOIN #mychannel
Send a channel message: PRIVMSG #mychannel :Hello, everyone!
Send a private message: PRIVMSG testuser2 :Hi!
Operator commands (if operator):
KICK #mychannel baduser
INVITE friend #mychannel
TOPIC #mychannel :New Topic
MODE #mychannel +i (or other modes: t, k, o, l)




Verify that channel messages are broadcast to all members and the client connects without errors.

With Netcat (nc) (Basic Testing)

Open a terminal and connect to the server:nc 127.0.0.1 6667


Send basic IRC commands (end with \r\n):PASS mypassword\r\n
NICK testuser\r\n
USER testuser 0 * :Test User\r\n


Check for server responses (e.g., welcome message).
Test edge cases (e.g., partial data, multiple clients).

Implementation Details

Non-blocking I/O: All sockets are set to non-blocking using fcntl(fd, F_SETFL, O_NONBLOCK), as required for macOS and project compliance.
Single poll() Call: Monitors all sockets (server and clients) for POLLIN events, ensuring efficient handling of connections and messages.
IRC Protocol: Implements core commands (NICK, USER, JOIN, PRIVMSG) and operator commands (KICK, INVITE, TOPIC, MODE) per RFC 1459.
Error Handling: Handles partial data, low bandwidth, disconnections, and invalid commands without crashing.
Channel Modes:
i: Invite-only channel.
t: Restrict topic changes to operators.
k: Set/remove channel password.
o: Grant/revoke operator privileges.
l: Set/remove user limit.


No Forking: Uses a single-threaded, event-driven model with poll().
C++ 98: Uses <string>, <vector>, and POSIX socket functions, avoiding C-style libraries like <string.h> where possible.

Limitations

Does not support server-to-server communication (not required).
Limited to IPv4 (IPv6 support optional).
Bonus features (file transfer, bot) not implemented in this version.
Error handling is robust but may need tuning for extreme edge cases.

Notes

The server is compatible with the reference client (HexChat) and follows standard IRC server behavior.
Avoids forbidden practices (e.g., forking, external libraries, blocking I/O without poll()).
The Makefile ensures no relinking and compiles with strict C++ 98 standards.
Test thoroughly with the reference client and nc to verify all features.

Author
Created as part of the ft_irc project for learning network programming and IRC protocol implementation.
For questions or contributions, please contact the repository owner or submit a pull request.
