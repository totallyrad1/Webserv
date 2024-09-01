#include "../../inc/Webserv.hpp"

void Server::insert_server(Config server_config)
{
	servers.push_back(server_config);
}

std::vector<Config> Server::get_servers() const{
	return this->servers;
}