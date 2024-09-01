#pragma once
#include "Webserv.hpp"

class CgiHandler {
	public:
		CgiHandler(Request &request, const std::string& body);
		CgiHandler();

		void		executeCgi(const std::string &scriptName);
        const std::string	&getResBody() const;
        int                 getCode() const;
		int getCgiStatus();
	private:
		std::map<std::string, std::string>	_env;
		char								**_envArr;
		std::string							_body;
		std::string							_res_body;
        int                                 _code;
        int                                 _timeout;
		pid_t		pid;
		int			saveStdin;
		int			saveStdout;
		int		fdIn;
		int		fdOut;
		int _bytesRead;
		int _childState;// 0 not created, 1 running, 2 finished
		void								_getEnvAsCstrArray() ;
		void								_initEnv(Request &request);
		void								cleanCgi();
};
