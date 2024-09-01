#include "../../inc/Webserv.hpp"

/*** TORS **/

void Request::initReq()
{
	this->_method = "";
	this->_path = "";
	this->_pub_path = "";
	this->_version = "";
	this->_headers.clear();
	this->_env_for_cgi.clear();
	this->_ret = 200;
	this->_query = "";
	this->_methods = _config.getAllowedMethods();
	this->_dirlst = false;
	this->_def_files.clear();
	this->_cgi_extensions.clear();
	this->_upload = false;
	this->_upload_path = "";
	this->_redirection = "";
	this->_isCgi = false;
	this->_max_body_size = _config.get_max_body_size();
	this->_has_max_body_size = _config.get_has_max_body_size();
	this->_header_line = "";
	this->_content_length = 0;
	this->_isChunked = false;
	this->_cgiTimeout = 0;
	this->_isCgi = false;
	this->_isChunked = false;
	this->_location_header = "";
}

Request::Request(const std::string &head, Config config)
	: _config(config), _head(head)
{
	_content_length = 0;
	this->initReq();
	this->parse();
}

Request &Request::operator=(const Request &src)
{
	this->_content_length = src._content_length;
	this->_method = src._method;
	this->_path = src._path;
	this->_pub_path = src._pub_path;
	this->_version = src._version;
	this->_headers = src._headers;
	this->_env_for_cgi = src._env_for_cgi;
	this->_ret = src._ret;
	this->_query = src._query;
	this->_methods = src._methods;
	this->_dirlst = src._dirlst;
	this->_def_files = src._def_files;
	this->_cgi_extensions = src._cgi_extensions;
	this->_upload = src._upload;
	this->_upload_path = src._upload_path;
	this->_redirection = src._redirection;
	this->_isCgi = src._isCgi;
	this->_cgiTimeout = src._cgiTimeout;
	this->_max_body_size = src._max_body_size;
	this->_has_max_body_size = src._has_max_body_size;
	this->_header_line = src.getHeaderLine();
	this->_isChunked = src.isChunked();
	this->_location_header = src.getLocationHeader();
	return (*this);
}

Request::Request(const Request &src)
{
	*this = src;
}

Request::Request()
{
	this->initReq();
}

/*** GETTERS **/

const std::map<std::string, std::string> &Request::getHeaders() const
{
	return (this->_headers);
}

const std::map<std::string, std::string> &Request::getEnv() const
{
	return (this->_env_for_cgi);
}

const std::string &Request::getMethod() const
{
	return (this->_method);
}

const std::string &Request::getVersion() const
{
	return (this->_version);
}

int Request::getRet() const
{
	return (this->_ret);
}


const std::string &Request::getPath() const
{
	return (this->_path);
}

const std::string &Request::getPubPath() const
{
	return (this->_pub_path);
}

const std::string &Request::getQuery() const
{
	return (this->_query);
}

bool Request::isCgi() const
{
	return (this->_isCgi);
}


bool Request::isChunked() const
{
	return (this->_isChunked);
}

int Request::getCgiTimeout() const
{
	return (this->_cgiTimeout);
}

bool Request::isDirList(void) const
{
	return (this->_dirlst);
}

const std::set<std::string> &Request::getDefFiles() const
{
	return (this->_def_files);
}

const std::set<std::string> &Request::getCgiExtensions() const
{
	return (this->_cgi_extensions);
}

bool Request::isUpload(void) const
{
	return (this->_upload);
}
const std::string &Request::getUploadPath() const
{
	return (this->_upload_path);
}
const std::string &Request::getRedirection() const
{
	return (this->_redirection);
}

const std::string& Request::getHeaderLine() const
{
	return (_header_line);
}

const std::string& Request::getHost() const
{
	return (getHeaders().find("host")->second);
}

size_t Request::getCL() const
{
	return (_content_length);
}

std::string Request::getLocationHeader() const
{
	return (_location_header);
}

/*** SETTERS **/



void Request::setRet(int ret)
{
	this->_ret = ret;
}

void Request::setMethod(const std::string &method)
{
	this->_method = method;
}

