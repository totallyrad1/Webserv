#include "../../inc/Webserv.hpp"

Client::Client(int Fd){
    this->client_fd = Fd;
    ReadEvent = true;
    WriteEvent = false;
    ReadyToClose = false;
    LastInteraction = getTime();
}


Client::Client(const Client &src){
    this->client_fd = src.client_fd;
    this->WriteEvent = src.WriteEvent;
    this->ReadEvent = src.ReadEvent;
    this->response = src.response;
    this->request = src.request;
    this->LastInteraction = src.LastInteraction;
}
void Client::set_WriteEvent(bool flag){
    this->WriteEvent = flag;
}
void Client::set_ReadEvent(bool flag){
    this->ReadEvent = flag;
}
bool Client::get_ReadEvent(){
    return this->ReadEvent;
}
bool Client::get_WriteEvent(){
    return this->WriteEvent;
}

unsigned long Client::getLastInteraction(){
    return this->LastInteraction;
}
void Client::setLastInteraction(unsigned long time){
    this->LastInteraction = time;
}

Client::~Client(){
    // if(response){
    //     delete response;
    //     response = NULL;
    // }
    // close(client_fd);
}
void Client::set_Response_Ptr(Response new_Resp){
    response = new_Resp;
}
ss_request &Client::get_Request(){
    return this->request;
}
Response &Client::get_Response(){
    return this->response;
}

int Client::getFd(){
    return this->client_fd;
}

bool Client::getReadyToClose(){
    return this->ReadyToClose;
}
void Client::setReadyToClose(){
    this->ReadyToClose = true;
}