#pragma once

#include "Webserv.hpp"
#include "Config.hpp"

class	HttpResponse;
class	Route;
class Request
{
  private:
	Config _config;
	std::string _head;
	std::string _method;
	std::string _pub_path;
	std::string _path;
	std::string _version;
	std::map<std::string, std::string> _headers;
	std::map<std::string, std::string> _env_for_cgi;
	int _ret;
	size_t _max_body_size;
	bool _has_max_body_size;
	std::string _query;
	std::set<std::string> _methods;
	bool _isCgi;
	bool _isChunked;
	int _cgiTimeout;
	bool _dirlst;
	std::set<std::string> _def_files;
	std::set<std::string> _cgi_extensions;
	bool _upload;
	std::string _upload_path;
	std::string _redirection;
	std::string _header_line;
	std::string _location_header;
	size_t _content_length;

	/*** PRIVATE MEMBERS ***/
	void initReq();
	void parseHeaderLine(std::string &line);
	void parseMethod(std::string &method);
	void parsePath(std::string &path);
	void parseVersion(std::string &version);
	void parseQueryString(std::string &query);
	void addHeader(const std::string key, const std::string value);
	void parseHeaders(std::string &headers);
	bool translatePath(std::string &path, std::map<std::string, Route> routes);

  public:
	Request(const std::string &head, Config config);
	Request();
	Request(const Request &src);
	Request &operator=(const Request &src);
	/*** GETTERS ***/
	const std::string& getHeaderLine() const;
	const std::string& getHost() const;
	const std::map<std::string, std::string> &getHeaders() const;
	const std::map<std::string, std::string> &getEnv() const;
	const std::string &getMethod() const;
	const std::string &getVersion() const;
	int getRet() const;
	const std::string &getPath() const;
	const std::string &getPubPath() const;
	const std::string &getQuery() const;
	bool isCgi() const;
	bool isChunked() const;
	int getCgiTimeout() const;
	bool isDirList() const;
	const std::set<std::string> &getDefFiles() const;
	const std::set<std::string> &getCgiExtensions() const;
	bool isUpload() const;
	const std::string &getUploadPath() const;
	const std::string &getRedirection() const;
	size_t getCL() const;
	std::string getLocationHeader() const;
	/*** SETTERS **/
	void setRet(int);
	void setMethod(const std::string &method);
	/*** PUBLIC MEMBERS ****/
	void parse();
};
