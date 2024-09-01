#pragma once
#include "Webserv.hpp"

class Route
{
  private:
	std::set<std::string> methods;
	std::string root_dir;
	bool dirlst;
	std::set<std::string> def_file;
	std::set<std::string> cgi_extensions;
	int cgi_timeout;
	bool upload;
	std::string upload_path;
	std::string redirection;

  public:
	Route();
	std::set<std::string> get_methods() const;
	std::string get_roor_dir() const;
	bool get_dirlst() const;
	std::set<std::string> get_default_file() const;
	std::set<std::string> get_cgi_extensions() const;
	int get_cgi_timeout() const;
	bool get_upload() const;
	std::string get_upload_path() const;
	std::string get_redirection_url() const;
	void route_reset();
	int fill_attribute(const std::string &key, const std::string &value);
};
