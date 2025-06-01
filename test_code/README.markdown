# ft_irc_basic - Basic IRC Server (Test Code)

## Overview

`ft_irc_basic` is a simplified C++ 98 implementation of an IRC (Internet Relay Chat) server named `ircserv`. This test code serves as a learning foundation for the full `ft_irc` project, demonstrating core concepts like non-blocking I/O with `poll()`, TCP/IP socket communication, and multi-client handling. The server accepts multiple client connections, echoes received messages back with a "Server: " prefix, and provides a starting point for implementing full IRC protocol features (e.g., `NICK`, `JOIN`, operator commands).

## Features

- **Multi-Client Support**: Handles multiple clients concurrently using non-blocking sockets.
- **TCP/IP Communication**: Uses IPv4 for reliable data transfer.
- **Non-Blocking I/O**: Employs a single `poll()` call to monitor server and client sockets.
- **Basic Message Echoing**: Receives client messages and responds with "Server: [message]".
- **Error Handling**: Manages client disconnections and basic socket errors.
- **C++ 98 Compliance**: Uses only C++ 98 and POSIX socket functions (`socket`, `bind`, `listen`, `accept`, `recv`, `send`, `fcntl`, `poll`).
- **No External Libraries**: Pure C++ 98 implementation, adhering to `ft_irc` guidelines.

## Purpose

This test code is a minimal prototype to understand socket programming and non-blocking I/O for the `ft_irc` project. It does not yet implement full IRC protocol features but lays the groundwork for adding commands like `NICK`, `USER`, `JOIN`, `PRIVMSG`, and operator commands (`KICK`, `INVITE`, `TOPIC`, `MODE`).

## Requirements

- **Compiler**: C++ compiler supporting C++ 98 (e.g., `g++`, `clang++`).
- **OS**: Unix-based system (Linux, macOS).
- **Dependencies**: Standard C++ 98 and POSIX socket libraries.
- **Testing Tools**: `netcat` (`nc`) for basic testing; IRC clients (e.g., HexChat) for future extensions.

## Project Structure

```
ft_irc_basic/
├── Makefile          # Compiles the project
├── main.cpp          # Entry point, parses arguments, starts server
├── Server.hpp        # Server class declaration
├── Server.cpp        # Server implementation (socket setup, client handling)
└── README.md         # This file
```

### File Descriptions

#### `Makefile`
- **Purpose**: Compiles the project with C++ 98 standards and flags `-Wall -Wextra -Werror -std=c++98`.
- **Rules**:
  - `all`: Builds the `ircserv` executable.
  - `clean`: Removes object files.
  - `fclean`: Removes object files and executable.
  - `re`: Rebuilds the project.

#### `main.cpp`
- **Purpose**: Parses command-line arguments (`port`, `password`) and starts the server.
- **Functionality**:
  - Validates `port` (1024–65535) and ensures `password` is non-empty.
  - Creates a `Server` instance and calls `start()` to run the server.

#### `Server.hpp`
- **Purpose**: Declares the `Server` class with members and methods for socket management.
- **Key Members**:
  - `_port`: Port number for listening.
  - `_password`: Server password (stored but unused in this version).
  - `_server_fd`: Server socket file descriptor.
  - `_address`: `sockaddr_in` for TCP/IP configuration.
  - `_poll_fds`: Vector of `pollfd` structures for monitoring sockets.
  - `_client_buffers`: Vector of strings to store client messages.
- **Methods**:
  - Constructor/Destructor
  - `setupSocket()`: Initializes the server socket.
  - `acceptNewClient()`: Accepts new client connections.
  - `handleClient()`: Processes client messages.
  - `start()`: Runs the main server loop.

#### `Server.cpp`
- **Purpose**: Implements the `Server` class for socket setup, client handling, and event loop.
- **Functions**:
  - **Constructor**: Initializes `_port`, `_password`, and sets `_server_fd` to `-1`.
  - **Destructor**: Closes server and client sockets to prevent resource leaks.
  - **`setupSocket()`**:
    - Creates a TCP socket (`AF_INET`, `SOCK_STREAM`).
    - Sets non-blocking mode with `fcntl(F_SETFL, O_NONBLOCK)`.
    - Enables port reuse with `SO_REUSEADDR`.
    - Binds to the specified port and listens for up to 10 connections.
    - Adds server socket to `_poll_fds` with `POLLIN` for monitoring.
  - **`acceptNewClient()`**:
    - Accepts new client connections using `accept()`.
    - Sets client socket to non-blocking.
    - Adds client to `_poll_fds` with `POLLIN` and initializes an empty buffer in `_client_buffers`.
  - **`handleClient(int client_fd, int index)`**:
    - Reads data from a client using `recv()`.
    - Closes and removes the client on disconnection (`bytes_received <= 0`).
    - Appends data to `_client_buffers[index]`, logs it, and echoes back with "Server: ".
    - Clears the buffer after processing.
  - **`start()`**:
    - Calls `setupSocket()` to initialize the server.
    - Runs an infinite loop with `poll()` to monitor sockets for `POLLIN` events.
    - Handles new connections (via `acceptNewClient()`) or client data (via `handleClient()`).

## Compilation

1. Clone the repository:
   ```bash
   git clone <repository_url> ft_irc_basic
   cd ft_irc_basic
   ```
2. Compile with the Makefile:
   ```bash
   make
   ```
   Generates the `ircserv` executable. Uses flags: `-Wall -Wextra -Werror -std=c++98`.

## Usage

Run the server with a port and password:
```bash
./ircserv <port> <password>
```
- `<port>`: Port number (1024–65535, e.g., 6667).
- `<password>`: Non-empty string (unused in this version but required for syntax).

Example:
```bash
./ircserv 6667 mypassword
```

Output:
```
Server listening on port 6667
```

## Testing

### Using Netcat (`nc`)

1. Connect to the server:
   ```bash
   nc 127.0.0.1 6667
   ```
2. Type a message (e.g., `Hello`) and press Enter.
3. The server responds with `Server: Hello`.
4. Open multiple `nc` sessions to test multi-client support.

Example:
```bash
$ nc 127.0.0.1 6667
Hello
Server: Hello
```

### Using an IRC Client (Limited)

- Clients like HexChat can connect to `localhost:6667`, but this basic version only echoes messages and doesn’t support IRC commands (e.g., `NICK`, `USER`).
- For full IRC client testing, extend the code to handle IRC protocol commands.

## Extending the Project

To build the full `ft_irc` project, add:
- **IRC Protocol**: Parse commands like `NICK`, `USER`, `JOIN`, `PRIVMSG`.
- **Channel Management**: Support creating/joining channels and broadcasting messages.
- **Operator Commands**: Implement `KICK`, `INVITE`, `TOPIC`, `MODE` (modes: `i`, `t`, `k`, `o`, `l`).
- **Authentication**: Use the password for client verification.
- **Error Handling**: Handle partial data, low bandwidth, and edge cases.
- **Data Structures**: Add `Client` and `Channel` classes to manage state.

## Limitations

- Lacks full IRC protocol support (only echoes messages).
- Password is stored but unused.
- Basic error handling; needs enhancement for edge cases (e.g., partial data).
- Not compatible with IRC clients for full functionality yet.

## Notes

- Adheres to `ft_irc` requirements: C++ 98, non-blocking I/O, single `poll()` call, no external libraries, no forking.
- Uses `fcntl()` for non-blocking sockets, as allowed for macOS.
- Serves as a learning tool for socket programming and event-driven servers.

## Contributing

Submit issues or pull requests to the repository. Ensure changes comply with C++ 98 and project guidelines.

## Author

Developed as a test implementation for the `ft_irc` project to learn network programming and non-blocking I/O.

---
*Last updated: June 1, 2025*