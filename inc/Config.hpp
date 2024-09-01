#pragma once
#include "Webserv.hpp"

class Route;
class Config
{
  private:
	bool has_max_body_size;
	unsigned long max_body_size;
	std::string host;
	std::set<int> ports;
	std::map<int, std::string> error_pages;
	std::map<std::string, Route> routes;
	std::set<std::string> server_names;
	std::set<std::string> allowed_methods;

  public:
	Config();
	unsigned long get_max_body_size() const;
	std::string getHost() const;
	std::set<int> getPorts() const;
	std::map<int, std::string> getErrorPages() const;
	std::map<std::string, Route> getRoutes() const;
	std::set<std::string> getServerNames() const;
	std::set<std::string> getAllowedMethods() const;
	bool get_has_max_body_size() const;

	void set_allowed_methods(std::set<std::string> allowed_methods);
	void set_max_body_size(unsigned long size);
	void set_host(std::string host);
	void set_ports(std::set<int> ports);
	void set_errorpages(std::map<int, std::string> error_pages);
	void insert_route(std::string str, Route);
	void set_routes(std::map<std::string, Route> routes);
	void set_servernames(std::set<std::string> server_names);
	void set_has_max_body_size(int flag);
};
