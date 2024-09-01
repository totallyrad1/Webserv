#include "../../inc/Webserv.hpp"

/*** TORS ***/
void Response::initResponse()
{
	_body = "";
	_stop_reading = false;
	_is_ready = true;
	_is_cgi_ready = true;
	_last_file_fd = -2;
	_out_fd = -2;
	is_file = false;
	_response_headers = "";
	_response_body = "";
	_response_path = "";
	_created = false;
	_type = "text/html";
	_content_length = _request.getCL();
	_chunks_length = 0;
	_chunked = _request.getHeaders().find("transfer-encoding") != _request.getHeaders().end() && _request.getHeaders().find("transfer-encoding")->second == "chunked";
	_code = _request.getRet();
	_def_files = _request.getDefFiles();
	_upload = _request.isUpload();
	_upload_path = _request.getUploadPath();
	_redirection = _request.getRedirection();
	_path = _request.getPath();
	_pub_path = _request.getPubPath();
	_isAutoIndex = _request.isDirList();
	_method = _request.getMethod();
	if (_request.getHeaders().find("content-type") != _request.getHeaders().end())
		_content_type = _request.getHeaders().find("content-type")->second;
	else
		_content_type = "text/html";
	if (_request.getHeaders().find("connection") != _request.getHeaders().end() && _request.getHeaders().find("connection")->second.find("keep-alive") != std::string::npos)
		_connection = 1;
	else
		_connection = 0;
	_isCgi = _request.isCgi();
	if (_isCgi)
	{
		_cgi = CgiHandler(_request, _body);
		_cgi_extensions = _request.getCgiExtensions();
	}
	_location_header = _request.getLocationHeader();
}

Response::Response(Request request, Config config):_request(request)

{
	_errorPages = config.getErrorPages();
	_server = request.getHeaders().find("host")->second;
	initResponse();
}

Response::~Response()
{
	_errorPages.clear();
	_def_files.clear();
	_cgi_extensions.clear();
	close(_last_file_fd);
	close(_out_fd);
}

Response::Response()
{
	_content_length = 0;
	_is_ready = true;
	_is_cgi_ready = true;
	_type = "text/html";
	_last_file_fd = -2;
	_out_fd = -2;
	is_file = false;
	_response_headers = "";
	_response_body = "";
	_response_path = "";
	_created = false;
}

/*** GETTERS ***/

std::string Response::getResponseHeaders()
{
	return (_response_headers);
}

std::string Response::getResponseBody()
{
	return (_response_body);
}

std::string Response::getResponsePath()
{
	return (_response_path);
}

bool Response::getConection()
{
	return (_connection);
}

bool Response::isReady() const
{
	// 1 = keep-alive, 0 = close
	return (_is_ready);
}

bool Response::isCgiReady() const
{
	return (_is_cgi_ready);
}

bool Response::stopReading()
{
	return (_stop_reading);
}

std::string Response::getBody()
{
	return (_body);
}

void Response::reset(){
	_errorPages.clear();
	_def_files.clear();
	_cgi_extensions.clear();
	close(_last_file_fd);
	close(_out_fd);
	initResponse();
}
