#pragma once 
#include "Webserv.hpp"

class Client;
class Webserver{
	private:
		std::map<int, Config> webserver;
		std::map<std::string, Config> _for_hostnames;
		std::set<int> sockets;
		std::set<int> ports_used;
		struct timespec timeout;
		std::map<int, int> clients;

		std::map <int , Client> _Clients;
		int kq;
		struct	kevent events[OPEN_MAX];
 	public:
		Webserver();
		void inset_configandfd(int socket_fd, Config config);
		// void inset_fdandstruct(int socket_fd, struct sockaddr_in address);
		bool check_used_port(int port);
		void insert_port(int port);
		void insert_server_sock(int fd);
		void insert_host_config(std::string host, Config config);
		bool check_if_server(int fd);
		bool check_if_client(int fd);
		void rmandclose_client(int fd);
		void close_allfds();
		void insert_client(int client_fd, int server_fd);
		std::map<int, Config> get_sockets();
		Config get_client_config(int client_fd, std::string hostname);
		std::map<int, int> get_clients();

		//// ******** server functions ***///////
		void register_ServerFds();
		// void fill_Read_Write_fds();
		void start_server();

		void accept_new_connection(int Fd);
		void read_from_client(int Fd);
		void send_to_client(int Fd);

		void remove_client_events(int Fd);
		void kick_clients();

		void timeout_monitor();
};
