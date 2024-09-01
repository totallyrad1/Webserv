# Webserv

**webserv** is a lightweight HTTP web server built in C++ as part of the 42 school curriculum. The project aims to deepen understanding of network programming, the HTTP protocol, and server management through the implementation of a custom web server from scratch.

# Features 

- **HTTP/1.1 Compliant**: Supports basic HTTP methods including GET, POST, and DELETE.
- **Multi-client Handling**: Efficiently manages multiple clients concurrently using I/O multiplexing.
- **Configurable**: Parses and handles configurations from a file, allowing customization of server behavior including host, port, server names, routes, and error pages.
- **Static and Dynamic Content**: Serves static files and can handle CGI scripts for dynamic content.
- **Error Handling**: Responds with appropriate HTTP status codes and custom error pages.
- **Chunked Transfer Encoding**: Supports chunked transfer encoding for streaming large responses.
- **Request Parsing**: Robust request parser that handles headers, body, and various content types.

# Learning Objectives

- Mastery of **socket programming** and understanding of TCP/IP.
- Deep dive into **HTTP/1.1** and its various components.
- Hands-on experience with **I/O multiplexing** techniques such as **select**, **poll**, or **kqueue**.
- Understanding of **CGI** and dynamic content generation.
- Managing **memory** and resources effectively in C++.

# Project Structure

- **`src/`**: Contains the main source code of the server.
- **`inc/`**: Header files and class definitions.
- **`configs/`**: Example configuration files.
- **`www/`**: Default directory for serving static files.
- **`www/cgi/`**: Directory for CGI scripts.

# Usage

1. **Compilation**:
   ```bash
   make
   ```

2. **Running the Server**:
    ```bash
    ./webserv path/to/config/file
    ```

3. **Testing**:
    ```bash
    siege -b http://host:port
    ```

# Requirements

- **C++98 Standard**: The server is built using C++98, adhering to the project constraints.
- **POSIX-Compatible OS**: Developed and tested on macOS, should work on any Unix-like system.

# Contributing

Contributions are welcome! Feel free to fork the repository, create a branch, and submit a pull request.

# License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.