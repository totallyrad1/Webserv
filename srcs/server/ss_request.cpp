#include "../../inc/Webserv.hpp"

ss_request::ss_request(){
			body_length = 0;
			bytes_read_from_body = 0;
			error = 0;
			content_lenght = -1;
			chunked = 0;
			chunk_lenght = -1;
			header_finished  = 0;
			request_ready = 0;
		}

void ss_request::parse_request_header(){
	if(header.find("\r\n\r\n") != std::string::npos){
		header_finished = 1;
		if(header.substr(0, 4) != "POST")
			request_ready = 1;
	}
}

void ss_request::parse_request(char *buffer, int bytes_read){
	if(header_finished == 0){
		header.append(buffer, bytes_read);
		parse_request_header();
	}
}

std::string ss_request::get_host(){
	if(header.find("Host:") != std::string::npos){
		int index = header.find("Host:");
		index += 5;
		int start;
		while(header[index] == ' ' || header[index] == '\t')
			index++;
		start = index;
		while(header[index] != ':' && header[index] != '\r' && header[index] != '\n' && header[index] != ' ' && header[index] != '\t'){
			index++;
		}
		std::string host = header.substr(start, index - start);
		return host;
	} 
	return "";
}

int ss_request::is_request_ready()const {
	return request_ready;
}
int ss_request::ret_error()const{
	return error;
}
std::string ss_request::what_method() const{
	return method;
}
std::string ss_request::get_header() const{
	return this->header;
}
std::string ss_request::get_body() const{
	return this->body;
}

void ss_request::reset(){
	error = 0;
	content_lenght = -1;
	chunked = 0;
	chunk_lenght = -1;
	header_finished  = 0;
	request_ready = 0;
	header = "";
	body = "";
	temp = "";
	method = "";
	boundary = "";
	bytes_read_from_body = 0;
	body_length = 0;
}

int ss_request::get_header_ready(){
	return this->header_finished;
}

void ss_request::set_header_finished(int flag){
	header_finished = flag;
}

size_t ss_request::getbody_length(){
	return this->body_length;
}
size_t ss_request::get_bytes_read_from_body(){
	return this->bytes_read_from_body;
}
void ss_request::set_body_length(size_t size){
	this->body_length = size;
}
void ss_request::add_bytes_read_from_body(size_t bytes_read){
	this->bytes_read_from_body += bytes_read;
}

void ss_request::set_request_ready(){
	request_ready = 1;
}