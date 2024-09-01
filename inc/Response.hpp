#pragma once
#include "Webserv.hpp"
#include "Request.hpp"
#include "CgiHandler.hpp"

class	Request;
class	CgiHandler;
class	Config;
class	Route;
class Response
{
	private:
		Request _request;
		CgiHandler _cgi;
		std::map<int, std::string> _errorPages;
		std::set<std::string> _cgi_extensions;
		std::set<std::string> _def_files;
		std::string _response_headers;
		std::string _content_type;
		std::string _response_body;
		std::string _response_path;
		std::string _upload_path;
		std::string _redirection;
		std::string _pub_path;
		std::string _method;
		std::string _server;
		std::string _path;
		std::string _body;
		std::string _type;
		std::string _location_header;
		std::string send_temp; // hada dial rad
		int _last_file_fd;
		int _out_fd;
		int _code;
		bool _isAutoIndex;
		bool _connection;
		bool _is_ready;
		bool _is_cgi_ready;
		bool _upload;
		bool is_file;
		bool _isCgi;
		bool _created;
		bool _chunked;
		bool _stop_reading;
		size_t _content_length;
		size_t _chunks_length;
		/*** PRI MEMBERS ***/
		void initResponse();
		void serveFile(std::string path);
		void handleGetReq();
		void handlePostReq();
		void handleDeleteReq();
		void setResPage();
		void uploadForm(std::string &_body);
		void uploadBody(std::string &_body);
		void uploadChunked();
		int dropFile(std::string path);
		int dropDirectory(std::string path);
	public :
		/*** TORS ***/
		Response(Request request, Config config);
		Response();
		~Response();
		/*** GETTERS ***/
		std::string getBody();
		std::string getResponseHeaders();
		std::string getResponseBody();
		std::string getResponsePath();
		bool isReady() const;
		bool isCgiReady() const;
		bool getConection();
		bool stopReading();
		/*** PUB MEMBERS ***/
		int sendResponse(int client_fd);
		int sendHeader(int client_fd);
		int sendfromFile(int client_fd);
		int sendfromBody(int client_fd);
		void uploadFile();
		void handleCgi();
		void reset();
		void handleResponse(char *body, size_t size);
};
