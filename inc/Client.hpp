#pragma once
#include "Webserv.hpp"
#include "Response.hpp"
#include "ss_request.hpp"

class Response;
class ss_request;

class Client{
    private:
        int client_fd;
        ss_request request;
        Response response;
        bool WriteEvent;
        bool ReadEvent;
        bool ReadyToClose;
        unsigned long LastInteraction;
    public:
        Client(int Fd);
        Client(const Client &obj);
        void set_WriteEvent(bool flag);
        void set_ReadEvent(bool flag);
        bool get_ReadEvent();
        bool get_WriteEvent();
        bool getReadyToClose();
        void setReadyToClose();
        unsigned long getLastInteraction();
        void setLastInteraction(unsigned long time);

        int getFd();
        ~Client();
        void set_Response_Ptr(Response new_Resp);
        ss_request &get_Request();
        Response &get_Response();
        
};
