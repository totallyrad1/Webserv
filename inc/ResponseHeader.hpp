#pragma once
#include "Webserv.hpp"

class ResponseHeader
{
  public:
	/*** CONSTRUCTOR ***/
	ResponseHeader(size_t length, const std::string &type, int code,
			std::string name, bool connection, std::string location_header);
	/*** SETTERS ***/
	void setContentLength(size_t length);
	void setContentType(std::string type);

	/*** GETTERS ***/
	std::string getHeader();

  private:
	std::string _content_length;
	std::string _content_type;
	std::string _server;
	std::string _connection;
	std::string _location_header;
	int _code;

	/*** SHADOWS ***/
	std::string writeHeaders(void);
};
