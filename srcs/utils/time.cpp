#include "../../inc/Webserv.hpp"

long getTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void printTime() {
    std::time_t now = std::time(0); 
    std::tm* local_time = std::localtime(&now);
    std::cout <<GREEN<< "[" 
              << (local_time->tm_year + 1900) << '-' 
              << (local_time->tm_mon + 1) << '-' 
              << local_time->tm_mday << ' ' 
              << local_time->tm_hour << ':' 
              << local_time->tm_min << ':' 
              << local_time->tm_sec << "] ";
}

void log(std::string msg, log_type type) {
    if (type == LOG)
    {
        printTime();
        std::string server = msg.substr(0, msg.find("<-"));
        std::string req = msg.substr(msg.find("<-") + 3);
        std::cout << PURPLE<< server;
        std::cout << YELLOW << "<- ";
        std::cout <<  PURPLE<<req;
        std::cout<< RESET << std::endl;
    }
    else if (DEBUG)
    {
        printTime();
        if (type == ERROR)
            std::cout << RED << msg ;
        else if (type == SUCCESS)
            std::cout << YELLOW << msg ;
        else
            std::cout << msg;
        std::cout<< RESET << std::endl;
    }
}