#include "../../inc/Webserv.hpp"

// DEBUG
Config::Config()
{
	this->max_body_size = 0;
	this->host = "";
	this->ports.clear();
	this->error_pages.clear();
	this->routes.clear();
	this->server_names.clear();
	this->allowed_methods.clear();
	this->has_max_body_size = false;
}
// DEBUG

std::string Config::getHost() const
{
	return (this->host);
}
std::set<int> Config::getPorts() const
{
	return (this->ports);
}
std::map<int, std::string> Config::getErrorPages() const
{
	return (this->error_pages);
}
std::map<std::string, Route> Config::getRoutes() const
{
	return (this->routes);
}
std::set<std::string> Config::getServerNames() const
{
	return (this->server_names);
}

void Config::set_host(std::string host)
{
	this->host = host;
}
void Config::set_ports(std::set<int> ports)
{
	this->ports = ports;
}
void Config::set_errorpages(std::map<int, std::string> error_pages)
{
	this->error_pages = error_pages;
}
void Config::set_routes(std::map<std::string, Route> routes)
{
	this->routes = routes;
}
void Config::set_servernames(std::set<std::string> server_names)
{
	this->server_names = server_names;
}

unsigned long Config::get_max_body_size() const
{
	return (this->max_body_size);
}

void Config::set_max_body_size(unsigned long size)
{
	this->max_body_size = size;
}

std::set<std::string> Config::getAllowedMethods() const
{
	return (this->allowed_methods);
}
void Config::set_allowed_methods(std::set<std::string> allowed_methods)
{
	this->allowed_methods = allowed_methods;
}


void Config::insert_route(std::string str, Route route){
	routes.insert(std::make_pair(str, route));
}

bool Config::get_has_max_body_size() const{
	return this->has_max_body_size;
}

void Config::set_has_max_body_size(int flag){
	this->has_max_body_size = flag;
}