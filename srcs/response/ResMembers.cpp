#include "../../inc/Webserv.hpp"

/*** PRI MEMBERS ***/

void Response::handleResponse(char *body, size_t size)
{
	if (_code == 301)
	{
		_stop_reading = true;
		ResponseHeader header(0,"", _code, _server, "", _location_header);
		_response_headers = header.getHeader();
		log("Redirecting to : " + _location_header, SUCCESS);
		return;
	}
	_body.append(body, size);
	_chunks_length += size;
	if (_chunks_length >= _content_length && !_chunked)
		_stop_reading = true;

	if (_code >= 400)
		return (setResPage());
	if (_method == "GET")
	{
		handleGetReq();
	}
	else if (_method == "POST")
	{
		handlePostReq();
	}
	else if (_method == "DELETE")
	{
		handleDeleteReq();
	}
	else
	{
		_code = 405;
		setResPage();
	}
}

void Response::handleDeleteReq()
{
	std::string file;
	if (_code == 200)
	{
		if (isDir(_path))
		{
			DIR *dir = opendir(_path.c_str());
			if (dir == NULL)
			{
				if (errno == 13)
					_code = 403;
				else
					_code = 500;
				log("Error opening directory " + _path, ERROR);
				setResPage();
				return;
			}
			closedir(dir);
			if ((file = searchDir(_path, _def_files)) != "" && _isCgi && _cgi_extensions.find(getExtenson(file)) != _cgi_extensions.end())
			{
				_path += "/" + file;
				_code = checkFileAccess(_path);
				if (_code == 200)
					handleCgi();
				else
					setResPage();
				return;
			}
			else
			{
				dropDirectory(_path);
				setResPage();
			}
		}
		else
		{
			_code = checkFileAccess(_path);
			if (_code == 200)
			{
				if (_isCgi && _cgi_extensions.find(getExtenson(_path)) != _cgi_extensions.end())
					handleCgi();
				else
					dropFile(_path);
			}
			else
			{
				log("Error opening file " + _path, ERROR);
				setResPage();
			}
		}
	}
	else
		setResPage();
}

int Response::dropFile(std::string path)
{
	if (remove(path.c_str()) != 0)
	{
		log("Error deleting file " + path, ERROR);
		_code = 500;
	}
	else
	{
		_code = 204;
	}
	setResPage();
	return _code;
}

int Response::dropDirectory(std::string path)
{
	DIR *dir;
    struct dirent *ent;
	_code = 204;
    if ((dir = opendir(path.c_str())) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
			if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
				continue;
			if (DT_DIR == ent->d_type)
            	_code = dropDirectory(path + "/" + ent->d_name);
			else
			{
				_code = dropFile(path + "/" + ent->d_name);
				if (_code != 204)
					break;
			}
        }
        closedir(dir);
    }
	else
		_code = 500;
	if (_code == 204)
	{
		if(dropFile(path) != 204)
		{
			log("Error deleting directory " + path, ERROR);
			_code = 500;
		}
		else
		{
			log("Directory deleted " + path, SUCCESS);
		}
	}
	return _code;
}

void Response::handlePostReq()
{
	std::string file;
	if (_code == 200)
	{
		if (!isDir(_path))
		{
			if (_isCgi && _cgi_extensions.find(getExtenson(_path)) != _cgi_extensions.end())
			{
				_code = checkFileAccess(_path);
				if (_code == 200)
					handleCgi();
				else
					setResPage();
				return;
			}
		}
		else
		{
			DIR *dir = opendir(_path.c_str());
			if (dir == NULL)
			{
				if (errno == 13)
					_code = 403;
				else
					_code = 500;
				log("Error opening directory " + _path, ERROR);
				setResPage();
				return;
			}
			closedir(dir);
			if ((file = searchDir(_path, _def_files)) != "" && _isCgi && _cgi_extensions.find(getExtenson(file)) != _cgi_extensions.end())
			{
				_path += "/" + file;
				_code = checkFileAccess(_path);
				if (_code == 200)
					handleCgi();
				else
					setResPage();
				return;
			}
		}
		if (_upload)
		{
			uploadFile();
			setResPage();
		}
	}
	else
		setResPage();
}

void Response::handleGetReq()
{
	std::string file;
	if (_code == 200)
	{
		if (isDir(_path))
		{
			DIR *dir = opendir(_path.c_str());
			if (dir == NULL)
			{
				if (errno == 13)
					_code = 403;
				else
					_code = 500;
				log("Error opening directory " + _path, ERROR);
				setResPage();
				return;
			}
			closedir(dir);
			if ((file = searchDir(_path, _def_files)) != "")
			{
				_path += "/" + file;
				_code = checkFileAccess(_path);
				if (_code == 200)
				{
					if (_isCgi && _cgi_extensions.find(getExtenson(_path)) != _cgi_extensions.end())
						handleCgi();
					else
						serveFile(_path);
				}
				else
					setResPage();
			}
			else if (_isAutoIndex)
			{
				AutoIndex autoIndex(_path, _pub_path);
				std::string content = autoIndex.getIndex();
				ResponseHeader header(content.size(), _type, _code, _server, _connection, _location_header);
				_response_headers = header.getHeader();
				_response_body = content;
			}
			else
			{
				_code = 403;
				setResPage();
			}
		}
		else
		{
			_code = checkFileAccess(_path);
			if (_code == 200)
			{
				if (_isCgi && _cgi_extensions.find(getExtenson(_path)) != _cgi_extensions.end())
				{
					handleCgi();
				}
				else
					serveFile(_path);
			}
			else
			{
				setResPage();
			}
		}
	}
	else
		setResPage();
}

void Response::handleCgi()
{
	_is_cgi_ready = false;
	_cgi.executeCgi(_path);
	if (_cgi.getCgiStatus() != 2)
		return;
	_code = _cgi.getCode();
	if (_code == 200)
	{
		std::string res = _cgi.getResBody();
		if (res.find("\r\n\r\n") == std::string::npos)
		{
			_code = 500;
			setResPage();
			_is_cgi_ready = true;
			return;
		}
		_response_headers = "HTTP/1.1 " + to_string(_code) + " " + getStatusMessage(_code)
		+ "\r\n";
		_response_headers += res.substr(0, res.find("\r\n\r\n")+4);
		_response_body = res.substr(res.find("\r\n\r\n")+4);
		_response_path = "";
	}
	else
		setResPage();
	_is_cgi_ready = true;
}

void Response::serveFile(std::string path)
{
	_type = getType(path);
	off_t length = getFileSize(path.c_str()); 
	if (length == -1)
	{
		_code = 500;
		setResPage();
		return;
	}
	ResponseHeader header(length, _type, _code, _server, _connection, _location_header);
	_response_headers = header.getHeader();
	_response_body = "";
	if(length == 0)
		_response_path = "";
	else
		_response_path = path;
	_out_fd = -2;
}

void Response::setResPage()
{
	off_t length;
	std::string path = "";
	std::string content;

	if (_errorPages.find(_code) != _errorPages.end())
	{
		int code = checkFileAccess(_errorPages[_code]);
		if (code == 200)
		{
			path = _errorPages[_code];
			_type = getType(path);
			content = "";
			length = getFileSize(path.c_str());
			if (length == -1)
			{
				path = "";
				_code = 500;
				content = generateResPage(_code);
				length = content.size();
			}
		}
		else
		{
			_code = 500;
			content = generateResPage(_code);
			length = content.size();
		}
	}
	else
	{
		content = generateResPage(_code);
		length = content.size();
	}
	if (_code >= 400)
		_connection = false;
	ResponseHeader header(length, _type, _code, _server, _connection, _location_header);
	_response_headers = header.getHeader();
	_response_body = content;
	_response_path = path;
}

void Response::uploadForm(std::string &_body)
{
	std::istringstream ss(_body);
	std::string line;
	std::string filename;
	std::string boundary;
	if (_content_type.find("boundary=") == std::string::npos)
	{
		log("Invalid form format", ERROR);
		_code = 400;
		_is_ready = true;
		setResPage();
		return;
	}
	boundary = "--"+_content_type.substr(_content_type.find("boundary=") + 9);
	if (std::getline(ss, line))
	{
		if (line.find(boundary) != std::string::npos)
		{
			_body.erase(0, line.size() + 1);
			is_file = true;
			std::getline(ss, line);
			_body.erase(0, line.size() + 1);
			if (line.find("; filename=") != std::string::npos)
			{
				filename = line.substr(line.find("filename=") + 10);
				filename.erase(filename.size() - 2);
				if (filename.empty() || findChar(filename, UNSAFE_CHARS) || findChar(filename, "/;%"))
				{
					log("Bad filename", ERROR);
					_code = 400;
					_is_ready = true;
					setResPage();
					return;
				}
				_last_file_fd = open((_upload_path + filename).c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0666);
				if (_last_file_fd == -1)
				{
					log("Error creating file " + (_upload_path + filename), ERROR);
					_code = 500;
					_is_ready = true;
					setResPage();
					return;
				}
				_created = true;
				try
				{
					std::getline(ss, line);
					_body.erase(0, line.size() + 1);
					std::getline(ss, line);
					_body.erase(0, line.size() + 1);
				}
				catch(const std::exception& e)
				{
					log("Invalid form format", ERROR);
					_code = 400;
					_is_ready = true;
					setResPage();
					return;
				}
			}
			else
				is_file = false;
		}
		if (is_file)
		{
			if (_body == "\r\n")
			{
				_body.clear();
				return;
			}
			size_t len = _body.find("\r\n"+boundary);
			if (len == std::string::npos)
				len = _body.size();

			// log("Uploading file ...", SUCCESS);
            int byteswritten = write(_last_file_fd, _body.c_str(), len);
			if (byteswritten < 0)
			{
				log("Error writing to file", ERROR);
				_code = 500;
				_is_ready = true;
				setResPage();
				return;
			}
			if (len != _body.size())
				byteswritten+=2;
			_body.erase(0, byteswritten);
		}
		else
			_body.clear();
	}
}

void Response::uploadBody(std::string &_body)
{
	if (_last_file_fd == -2)
		_last_file_fd = open((_upload_path + "_clientFile" + to_string(getTime())).c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0666);
	if (_last_file_fd == -1)
	{
		_code = 500;
		log("Error creating file in upload store", ERROR);
		setResPage();
		return;
	}
	if (write(_last_file_fd, _body.c_str(), _body.size()) == -1)
	{
		_code = 500;
		log("Error writing to file in upload store", ERROR);
		setResPage();
		return;
	}
	_body.clear();
	_created = true;
	if (_stop_reading)
	{
		if (_created)
			_code = 201;
		else
		{
			log("No file to upload", ERROR);
			_code = 403;
		}
		if (_last_file_fd != -2)
			close(_last_file_fd);
		setResPage();
	}
}

void Response::uploadChunked()
{
	size_t chunk_size;
	std::string line = _body.substr(0, _body.find("\r\n"));
	try
	{
		chunk_size = Stol(line, 0, 16);
	}
	catch(const std::exception& e)
	{
		log("Invalid chunk size", ERROR);
		if (_last_file_fd != -2)
			close(_last_file_fd);
		_code = 400;
		setResPage();
		_stop_reading = true;
		return;
	}
	if (chunk_size == 0)
	{
		_stop_reading = true;
		return;
	}
	if (chunk_size + line.size() + 2 >= _body.size())
	{
		//not compelete chunk size
		return;
	}
	_body.erase(0, line.size() + 2);
	std::string chunk = _body.substr(0, chunk_size);
	_body.erase(0, chunk_size + 2);
	if (_content_type.find("multipart/form-data") != std::string::npos)
		uploadForm(chunk);
	else
		uploadBody(chunk);
	if (!_body.empty())
		uploadChunked();
}

/*** PUB MEMBERS ***/

int Response::sendHeader(int client_fd){
	int bytes_sent = send(client_fd, _response_headers.c_str(), _response_headers.size(), 0);
	if (bytes_sent < 0)
	{
		log("Error sending response headers", ERROR);
		return -1;
	}
	else if (bytes_sent >= static_cast<int>(_response_headers.length()))
		_response_headers = "";
	else
		_response_headers.erase(0, bytes_sent);
	return 0;
}

int Response::sendfromFile(int client_fd){
	
	if (_out_fd == -2){
		_out_fd = open(_response_path.c_str(), O_RDONLY);
	}
	if (_out_fd == -1)
	{
		log("Error opening file " + _response_path, ERROR);
		_out_fd = -2;
		return -1;
	}
	if(send_temp != ""){
		int bytes_sent = send(client_fd, send_temp.c_str(), send_temp.size(), 0);
		if (bytes_sent < 0)
		{
			close(_out_fd);
			_out_fd = -2;
			return -1;
		}else if (bytes_sent >= static_cast<int>(send_temp.length()))
			send_temp = "";
		else
			send_temp.erase(0, bytes_sent);
	}else{
		char buffer[SEND_BUFFER_SIZE + 1];
		int bytes_read;
		bytes_read = read(_out_fd, buffer, SEND_BUFFER_SIZE);
		if (bytes_read > 0)
		{
			buffer[bytes_read] = '\0';
			send_temp.append(buffer, bytes_read);
		}
		else if (bytes_read == 0)
		{
			_response_path = "";
			close(_out_fd);
			_out_fd = -2;
		}
		if (bytes_read == -1)
		{
			close(_out_fd);
			_out_fd = -2;
			_response_path = "";
			log("Error reading file " + _response_path, ERROR);
			return -1;
		}
	}
	return 0;
}

int Response::sendfromBody(int client_fd){
	int bytes_sent = send(client_fd, _response_body.c_str(), _response_body.size(), 0);
	if (bytes_sent < 0)
	{
		log("Error sending response body", ERROR);
		return -1;
	}
	else if (bytes_sent >= static_cast<int>(_response_body.length()))
		_response_body = "";
	else
		_response_body.erase(0, bytes_sent);
	return 0;
}

int Response::sendResponse(int client_fd)
{

	if (_response_headers != "")
	{
		if(sendHeader(client_fd) == -1){
			return -1;
		}
	}
	else if (_response_headers == "")
	{
		if (_response_body == "")
		{
			if (_response_path == "")
			{
				close(_out_fd);
				_out_fd = -2;
				return 1;
			}
			else
			{
				if(sendfromFile(client_fd) == -1){
					return -1;
				}
			}
		}
		else
		{
			if(sendfromBody(client_fd) == -1){
				return -1;
			}
		}
	}
	if (_response_headers == "" && _response_body == "" && _response_path == "")
	{
		close(_out_fd);
		_out_fd = -2;
		return 1;
	}
	return 0;
}

void Response::uploadFile()
{
	if (_chunked){
		uploadChunked();
	}
	else if (_content_type.find("multipart/form-data") != std::string::npos){
		uploadForm(_body);
	}
	else{
		uploadBody(_body);
	}
	if (_stop_reading)
	{
		if (_created)
		{
			_code = 201;
			log("File uploaded successfully", SUCCESS);
		}
		else
		{
			log("No file to upload", ERROR);
			_code = 403;
		}
		if (_last_file_fd != -2)
			close(_last_file_fd);
		_is_ready = true;
		setResPage();
	}
}
