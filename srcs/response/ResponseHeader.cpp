#include "../../inc/Webserv.hpp"

ResponseHeader::ResponseHeader(size_t length, const std::string &type, int code,
		std::string name, bool connection, std::string location_header)
{
	setContentLength(length);
	setContentType(type);
	_location_header = location_header;
	_server = name;
	_code = code;
	if (connection)
		_connection = "keep-alive";
	else
		_connection = "close";
}

void ResponseHeader::setContentLength(size_t length)
{
	if (length == 0)
		_content_length = "0";
	else
		_content_length = to_string(length);
}

void ResponseHeader::setContentType(std::string type)
{
	_content_type = type;
}

std::string ResponseHeader::writeHeaders(void)
{
	std::string cl = _content_length.empty() ? ""
												: "content-length: "
													+ _content_length + "\r\n";
	std::string ct = _content_type.empty() ? "" : "content-type: "
		+ _content_type + "\r\n";
	std::string server = _server.empty() ? "" : "Server: " + _server + "\r\n";
	std::string cnx = _server.empty() ? "" : "Connection: " + _connection + "\r\n";
	return (cl + ct + server + cnx);
}

std::string ResponseHeader::getHeader()
{
	std::string header;
	header = "HTTP/1.1 " + to_string(_code) + " " + getStatusMessage(_code)
		+ "\r\n";
	if (_code == 301)
		header += "Location: " + _location_header + "\r\n";
	else
		header += writeHeaders();
	header += "\r\n";
	return (header);
}
