#pragma once

/*** CPP Includes ***/
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

/*** CPP Containers ***/
#include <algorithm>
#include <list>
#include <map>
#include <set>
#include <vector>
#include <signal.h>

/*** C Includes ***/
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <netdb.h>
/*** C System ***/
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>

/*** C Network ***/
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <poll.h>
#include <sys/event.h>

/*** Colors ***/
#define RED "\033[1;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"
#define BLUE "\033[34m"
#define PURPLE "\033[35m"

/*** Macros ***/
#ifndef DEBUG
# define DEBUG 0
#endif

enum log_type {
    LOG,
    SUCCESS,
    ERROR
};

/*** Constants ***/
#define UNSAFE_CHARS "[]{}|\"~#<>"
#define SEND_BUFFER_SIZE 104200
#define BUFFER_SIZE 104200
#define UPLOAD_BUFFER_SIZE 9999999
#define CLIENT_TIMEOUT 180000 // 3 minutes

/*** CGI Constants ***/
#define CGI_BUFFER_SIZE 10240
#define CGI_TIMEOUT 2 // 2 seconds
#define SH_CMD "/bin/bash"
#define PY_CMD "/usr/local/bin/python3"
#define PHP_CMD "/usr/bin/php"
#define JAVA_CMD "/usr/bin/java"

/*** Custom Includes ***/
#include "Client.hpp"
#include "ss_request.hpp"
#include "Request.hpp"
#include "Config.hpp"
#include "Response.hpp"
#include "ResponseHeader.hpp"
#include "Route.hpp"
#include "utils.hpp"
#include "AutoIndex.hpp"
#include "CgiHandler.hpp"
#include "WebServer.hpp"
#include "Server.hpp"


// main //TODO:(set those to class attributes)
class Webserver;
void printConfig(const Config& config);
void parsing(Server &servers, std::string config_path);
void init_sockets(Webserver &webserver, const Server &servers);
long getTime();
