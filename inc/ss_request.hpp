#ifndef SS_REQUEST_HPP
#define SS_REQUEST_HPP
#include "Webserv.hpp"

class ss_request{
	private:
		size_t bytes_read_from_body;
		size_t body_length;
		std::string header;
		std::string body;
		std::string method;
		std::string temp;
		std::string boundary;
		int header_finished;
		int error;
		size_t content_lenght;
		bool chunked;
		int chunk_lenght;
		int request_ready;
	public:
		ss_request();
		void parse_request(char *buffer, int bytes_read);
		void parse_request_header();
		void parse_chunked_body(int flag);
		void parse_normal_body(int flag);
		void get_boundary();
		void set_request_ready();
		int is_request_ready()const;
		int ret_error()const;
		std::string what_method() const;
		std::string get_header() const;
		std::string get_body() const;
		void reset();
		std::string get_host();
		int get_header_ready();
		void set_header_finished(int flag);
		size_t getbody_length();
		size_t get_bytes_read_from_body();
		void set_body_length(size_t size);
		void add_bytes_read_from_body(size_t bytes_read);
};

#endif