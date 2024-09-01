#include "../../inc/Webserv.hpp"

/*** PUBLIC MEMBERS ****/

void Request::parse()
{
	_header_line = _head.substr(0, _head.find("\r\n"));
	log(_headers["host"]+ " <- " + _header_line, LOG);
	parseHeaderLine(_header_line);
	if (_ret != 200 && _ret != 301)
		return ;
	parseHeaders(_head);
	if (_headers.find("host") == _headers.end())
	{
		log("Host not found", ERROR);
		_ret = 400;
		return ;
	}
	if (_method == "GET")
		return ;
	if (_headers.find("transfer-encoding") != _headers.end())
	{
		if (_headers["transfer-encoding"].find("chunked") == std::string::npos)
		{
			log("Transfer-Encoding not supported", ERROR);
			_ret = 501;
			return;
		}
		_isChunked = true;
	}
	else  if (_headers.find("content-length") != _headers.end())
	{
		try
		{
			if (_has_max_body_size && (size_t)Stol(_headers["content-length"],0,10) > _max_body_size)
			{
				log("Body too large", ERROR);
				_ret = 413;
				return;
			}
			_content_length = Stol(_headers["content-length"],0,10) ;
		}
		catch(const std::exception& e)
		{
			log("Bad content-lenght "+ std::string(e.what()), ERROR);
			_ret = 400;
			return;
		}
		
	}
	else
		addHeader("content-length", "0");
}

/*** PRIVATE MEMBERS ***/

void Request::addHeader(const std::string key, const std::string value)
{
	_headers[key] = value;
}

void Request::parseMethod(std::string &method)
{
	if (_methods.find(method) == _methods.end())
	{
		log(method + " Method not allowed", ERROR);
		_ret = 405;
	}
	else
		_method = method;
}

bool	checkPathInRoot(std::string path)
{
	char	*res;
	int		pos;

	std::string tmp(path);
	res = strtok((char *)tmp.c_str(), "/");
	pos = 0;
	while (res != NULL)
	{
		if (!strcmp(res, ".."))
			pos--;
		else
			pos++;
		if (pos < 0)
		{
			return (false);
		}
		res = strtok(NULL, "/");
	}
	return (true);
}

bool Request::translatePath(std::string &path, std::map<std::string, Route> routes)
{
	_pub_path = path;
	std::string serverRoute = "";
	std::map<std::string, Route>::iterator route;
	for (route = routes.begin(); route != routes.end(); route++)
	{
		if (path.find(route->first) == 0 && (path.size() == route->first.size() || path[route->first.size()] == '/' || route->first == "/"))
		{
			if (route->first.size() > serverRoute.size())
				serverRoute = route->first;
		}
	}
	if (serverRoute.size())
	{
		Route route = routes[serverRoute];
		if (!route.get_redirection_url().empty())
		{
			_ret = 301;
			_location_header = route.get_redirection_url();
		}
		else
		{
			if (serverRoute == "/")
				findAndReplaceAll(path, serverRoute, route.get_roor_dir()+"/");
			else
				findAndReplaceAll(path, serverRoute, route.get_roor_dir());
			this->_dirlst = route.get_dirlst();
			this->_def_files = route.get_default_file();
			this->_cgi_extensions = route.get_cgi_extensions();
			this->_isCgi = this->_cgi_extensions.size() > 0;
			this->_cgiTimeout = route.get_cgi_timeout();
			this->_upload = route.get_upload();
			this->_upload_path = route.get_upload_path();
		}
		this->_methods = route.get_methods();
		parseMethod(_method);
		return (true);
	}
	return (false);
}

std::string urlDecode(const std::string &str)
{
	char	ch;

	std::string decoded;
	size_t i, ii;
	for (i = 0; i < str.length(); i++)
	{
		if (str[i] == '%')
		{
			std::stringstream ss;
			ss << std::hex << str.substr(i + 1, 2);
			ss >> ii;
			ch = static_cast<char>(ii);
			decoded += ch;
			i += 2;
		}
		else if (str[i] == '+')
		{
			decoded += ' ';
		}
		else
		{
			decoded += str[i];
		}
	}
	return (decoded);
}

void Request::parsePath(std::string &path)
{
	size_t	pos;
	path = reduce(path, '/');
	path = urlDecode(path);
	if (path.empty() || findChar(path, UNSAFE_CHARS))
	{
		log("Path: ("+path+") have unsafe chars", ERROR);
		_ret = 400;
		return ;
	}
	else if (path.size() > 2048)
	{
		log("Path too long", ERROR);
		_ret = 414;
	}
	else if (!checkPathInRoot(path))
	{
		log("Path not in root", ERROR);
		_ret = 403;
		return ;
	}
	else if (!translatePath(path, _config.getRoutes()))
	{
		log(path + " Path not found", ERROR);
		_ret = 404;
		return ;
	}
	pos = path.find("?");
	if (pos != std::string::npos)
	{
		_query = path.substr(pos + 1);
		path = path.substr(0, pos);
	}
	_path = reduce(path, '/');;
}

void Request::parseQueryString(std::string &query)
{
	size_t	pos;

	std::stringstream ss(query);
	std::string keyValuePair;
	while (std::getline(ss, keyValuePair, '&'))
	{
		pos = keyValuePair.find('=');
		if (pos != std::string::npos)
		{
			std::string key = keyValuePair.substr(0, pos);
			std::string value = keyValuePair.substr(pos + 1);
			_env_for_cgi[key] = value;
		}
	}
}

void Request::parseVersion(std::string &version)
{
	if (version != "HTTP/1.1" && version != "HTTP/1.0")
	{
		log(version + " Version not supported", ERROR);
		_ret = 505;
	}
	else
		_version = version;
}

void Request::parseHeaderLine(std::string &line)
{
	line = trim(line, "\r");
	std::string method = line.substr(0, line.find(" "));
	std::string path = line.substr(line.find(" ") + 1, line.rfind(" ")
			- line.find(" ") - 1);
	std::string version = line.substr(line.rfind(" ") + 1);
	parseMethod(method);
	if (_ret != 200 && _ret != 301)
		return ;
	parsePath(path);
	if (_ret != 200 && _ret != 301)
		return ;
	parseQueryString(_query);
	if (_ret != 200 && _ret != 301)
		return ;
	parseVersion(version);
}

void Request::parseHeaders(std::string &head)
{
	head = head.substr(_header_line.size() + 2);
	std::stringstream ss(head);
	size_t	pos;

	std::string line;
	while (std::getline(ss, line))
	{
		line = trim(line, "\r");
		if (line.empty())
			break ;
		pos = line.find(": ");
		if (pos != std::string::npos)
		{
			std::string key = toLower(line.substr(0, pos));
			std::string value = line.substr(pos + 2);
			addHeader(key, value);
		}
	}
	ss.clear();
	head.clear();
}

