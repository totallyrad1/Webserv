#include "../../inc/Webserv.hpp"

CgiHandler::CgiHandler(Request &request, const std::string& body)
{
    _body = body;
	_timeout = request.getCgiTimeout();
    _res_body = "";
    _code = 200;
	pid = -1;
	saveStdin = -1;
	saveStdout = -1;
	fdIn = -1;
	fdOut = -1;
	_bytesRead = 1;
	_childState = 0;
	this->_initEnv(request);
}

CgiHandler::CgiHandler()
{
	_timeout = 30;
    _body = "";
    _res_body = "";
    _code = 200;
	pid = -1;
	saveStdin = -1;
	saveStdout = -1;
	fdIn = -1;
	fdOut = -1;
	_bytesRead = 1;
	_childState = 0;
	_env = std::map<std::string, std::string>();
	_envArr = NULL;
}

void		CgiHandler::_initEnv(Request &request) {
	std::map<std::string, std::string>	headers = request.getHeaders();
	this->_env["SERVER_NAME"] = headers["host"];
	this->_env["GATEWAY_INTERFACE"] = "CGI/1337";
	this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_env["SERVER_PORT"] = this->_env["SERVER_NAME"].substr(this->_env["SERVER_NAME"].find(":") + 1);
	this->_env["REMOTE_ADDR"] = this->_env["SERVER_NAME"].substr(0, this->_env["SERVER_NAME"].find(":"));
	this->_env["REQUEST_METHOD"] = request.getMethod();
	this->_env["SCRIPT_NAME"] = request.getPath();
	this->_env["QUERY_STRING"] = request.getQuery();
    this->_env.insert(request.getEnv().begin(), request.getEnv().end());
	this->_env["CONTENT_TYPE"] = headers["content-type"].substr(0, headers["content-type"].find(";"));
	this->_env["CONTENT_LENGTH"] = to_string(this->_body.length());
	this->_env["HTTP_COOKIE"] = headers["cookie"];
}

void	CgiHandler::_getEnvAsCstrArray() {
	char	**env = new char*[this->_env.size() + 1];
	int	j = 0;
	for (std::map<std::string, std::string>::const_iterator i = this->_env.begin(); i != this->_env.end(); i++) {
		std::string	element = i->first + "=" + i->second;
		env[j] = new char[element.size() + 1];
		env[j] = strcpy(env[j], (const char*)element.c_str());
		j++;
	}
	env[j] = NULL;
	_envArr = (char**)env;
}

void		CgiHandler::executeCgi(const std::string& scriptName) {

	if (_childState == 0)
	{
		saveStdin = dup(STDIN_FILENO);
		saveStdout = dup(STDOUT_FILENO);
		fdIn = fileno(tmpfile());
		fdOut = fileno(tmpfile());
		try {
			_getEnvAsCstrArray();
		}
		catch (std::bad_alloc &e) {
			log("Bad alloc in getEnvAsCstrArray", ERROR);
			_code = 500;
			cleanCgi();
		}
		_childState = 1;
		
		if (write(fdIn, _body.c_str(), _body.size()) == -1)
		{
			log("Error writing to tmp file", ERROR);
			_code = 500;
			cleanCgi();
			return;
		}
		lseek(fdIn, 0, SEEK_SET);

		char *_file = (char*)scriptName.c_str();
		char *_caller = NULL;
		char *args[3];
		if (getExtenson(scriptName) == "sh")
			_caller = (char *)SH_CMD;
		else if (getExtenson(scriptName) == "out")
			_caller = _file;
		else if (getExtenson(scriptName) == "py")
			_caller = (char *)PY_CMD;
		else if (getExtenson(scriptName) == "php")
			_caller = (char *)PHP_CMD;
		else if (getExtenson(scriptName) == "java")
			_caller = (char *)JAVA_CMD;
		else
		{
			std::cerr << RED << "Not Implemented CGI extension." << RESET << std::endl;
			_code = 501;
			return;
		}
		args[0] = _caller;
		args[1] = _file;
		args[2] = NULL;
		pid = fork();
		if (pid == -1)
		{
			std::cerr << RED << "Fork crashed." << RESET << std::endl;
			_code = 500;
			cleanCgi();
			return;
		}
		_childState = 1;
		if (!pid)
		{
			alarm(_timeout);
			dup2(fdIn, STDIN_FILENO);
			dup2(fdOut, STDOUT_FILENO);
			int fd = open("/dev/null", O_WRONLY);
			dup2(fd, STDERR_FILENO);
			execve(_caller, args, _envArr);
			log("Execve crashed.", ERROR);
			exit(1);
		}
	}
	if (_childState == 1)
	{
		char	buffer[CGI_BUFFER_SIZE] = {0};
		int status;
		if (waitpid(pid, &status, WNOHANG) != 0)
		{
			if (WIFSIGNALED(status))
			{
				if (WTERMSIG(status) == SIGALRM)
				{
					log("CGI Timeout", ERROR);
					_code = 504;
				}
				else
				{
					log("CGI crashed.", ERROR);
					_code = 500;
				}
				cleanCgi();
				return;
			}
			else if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			{
				log("CGI crashed.", ERROR);
				_code = 500;
				_childState = 2;
				cleanCgi();
				return;
			}
			if (_res_body.empty())
				lseek(fdOut, 0, SEEK_SET);
			memset(buffer, 0, CGI_BUFFER_SIZE);
			_bytesRead = read(fdOut, buffer, CGI_BUFFER_SIZE - 1);
			_res_body += buffer;
			if (_bytesRead < CGI_BUFFER_SIZE)
				cleanCgi();
		}	
	}
}

void CgiHandler::cleanCgi() {
	_childState = 2;
	dup2(saveStdin, STDIN_FILENO);
	dup2(saveStdout, STDOUT_FILENO);
	close(fdIn);
	close(fdOut);
	close(saveStdin);
	close(saveStdout);
	if (_envArr)
	{
		for (int i = 0; _envArr[i]; i++)
			delete[] _envArr[i];
		delete[] _envArr;
	}
}

int CgiHandler::getCgiStatus() {
	return _childState;
}

int CgiHandler::getCode() const {
    return _code;
}

const std::string &CgiHandler::getResBody() const {
    return _res_body;
}