#ifndef SERVER_HPP
#define SERVER_HPP
#include "Webserv.hpp"

class Server{
	private:
		std::vector<Config> servers;
	public:
		void insert_server(Config server_config);
		std::vector<Config> get_servers() const;
		
};

#endif