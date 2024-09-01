#pragma once
#include "Webserv.hpp"

class AutoIndex
{
    private:
        std::string _path;
        std::string _pub_path;
    
    public:
        AutoIndex(std::string path, std::string pub_path);
        ~AutoIndex();
        std::string getIndex();
};
