#include "../../inc/Webserv.hpp"

Webserver::Webserver(){
	timeout.tv_nsec = 0;
	timeout.tv_sec = 5;
}

void Webserver::inset_configandfd(int socket_fd, Config config){
	webserver.insert(std::make_pair(socket_fd, config));
}

bool Webserver::check_used_port(int port){
	std::set<int>::iterator it = ports_used.find(port);
	if(it != ports_used.end())
		return 0;
	else
		return 1;
}
void Webserver::insert_port(int port){
	ports_used.insert(port);
}

void Webserver::close_allfds(){
	std::map<int, Config>::iterator it = webserver.begin();
	while(it != webserver.end())
	{
		close(it->first);
		++it;
	}
	std::map<int, int>::iterator it1 = clients.begin();
	while(it1 != clients.end()){
		close(it1->first);
		++it1;
	}
	close(kq);
}

void Webserver::insert_server_sock(int fd){
	sockets.insert(fd);
}

bool Webserver::check_if_server(int fd){
	std::map<int, Config>::iterator it = webserver.find(fd);
	if(it != webserver.end())
		return true;
	else
		return false;
}

bool Webserver::check_if_client(int fd){
	std::map<int, int>::iterator it = clients.find(fd);
	if(it != clients.end())
		return true;
	else
		return false;
}

void Webserver::insert_client(int client_fd, int server_fd){
	clients.insert(std::make_pair(client_fd,server_fd));
}

std::map<int, Config> Webserver::get_sockets(){
	return this->webserver;
}

void Webserver::rmandclose_client(int fd){
	// close(fd);
	clients.erase(fd);
}

Config Webserver::get_client_config(int client_fd, std::string hostname){
	std::map<std::string, Config>::iterator it = _for_hostnames.find(hostname);
	if(it != _for_hostnames.end()){
		return it->second;
	}
	int server_fd = clients[client_fd];
	return webserver[server_fd];
}

void Webserver::insert_host_config(std::string host, Config config){
	_for_hostnames.insert(std::make_pair(host, config));
}

std::map<int, int> Webserver::get_clients(){
	return this->clients;
}

void Webserver::timeout_monitor(){
	for(std::map<int, Client>::iterator it = _Clients.begin(); it != _Clients.end();){
		if(getTime() - it->second.getLastInteraction() > CLIENT_TIMEOUT){
			log("Client (" + to_string(it->first) + ") TIMEOUT", ERROR);
			it->second.get_Response().reset();
			it->second.get_Request().reset();
			close(it->second.getFd());
			it->second.set_ReadEvent(0);
			it->second.set_WriteEvent(0);
			close(it->first);
			it = _Clients.erase(it);
		}else{
			it++;
		}
	}
}

///********* SERVER FUNCTIONS ******///////

void register_read(int kq, int fd){
    struct kevent change;
    EV_SET(&change, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
	kevent(kq, &change, 1, NULL, 0, NULL);
}

void register_write(int kq, int fd){
    struct kevent change;
    EV_SET(&change, fd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
	kevent(kq, &change, 1, NULL, 0, NULL);
}

void delete_read(int kq, int fd){
    struct kevent change;
    EV_SET(&change, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
	kevent(kq, &change, 1, NULL, 0, NULL);
}

void delete_write(int kq, int fd){
    struct kevent change;
    EV_SET(&change, fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
	kevent(kq, &change, 1, NULL, 0, NULL);
}

void Webserver::register_ServerFds(){
	for(std::map<int, Config>::iterator it = webserver.begin(); it != webserver.end(); ++it){
        register_read(kq, it->first);
	}
}

void Webserver::kick_clients(){
	for(std::map <int , Client>::iterator it = _Clients.begin(); it != _Clients.end();){
		if(!it->second.get_WriteEvent() && !it->second.get_ReadEvent()){
			log("Client (" + to_string(it->first) + ") Disconnected", SUCCESS);
			it->second.get_Response().reset();
			delete_read(kq, it->first);
			delete_write(kq, it->first);
			close(it->first);
			it = _Clients.erase(it);
		}
		else
			it++;
	}
}

void Webserver::remove_client_events(int Fd){
	std::map<int, Client>::iterator it = _Clients.find(Fd);
	if(it != _Clients.end()){
		it->second.get_Response().reset();
		close(it->first);
		it->second.set_ReadEvent(0);
		it->second.set_WriteEvent(0);
	}
}

void Webserver::start_server(){
	kq = kqueue();
	if(kq < 0){
		perror("kqueue");
		close_allfds();
		exit(1);
	}
	register_ServerFds();
	for(;;)
	{
		kick_clients();
		memset(events, 0, sizeof(events));
		int ret = kevent(kq, NULL, 0, &events[0], sizeof(events), &timeout);
		if(ret < 0){
			close(kq);
			close_allfds();
			perror("kevent");
			exit(1);
		}else if(ret == 0)
			timeout_monitor();
		else{
			for(int i = 0; i < ret; i++){
				if(events[i].flags & EV_EOF)
					remove_client_events(events[i].ident);
				else if(events[i].filter == EVFILT_READ){
					int socketfd = events[i].ident;
					if(check_if_server(socketfd))
						accept_new_connection(socketfd);
					else if(check_if_client(socketfd))
						read_from_client(socketfd);
				}else if(events[i].filter == EVFILT_WRITE)
				{
					send_to_client(events[i].ident);
				}
			}
		}
	}
}

void Webserver::accept_new_connection(int Fd){
	struct sockaddr_storage remoteaddr;
	unsigned int remotaddrlen = sizeof(remoteaddr);
	int new_fd = accept(Fd, (struct sockaddr *)&remoteaddr, &remotaddrlen);
	if(new_fd < 0){
		perror("accept");
	}else{
		if (fcntl(new_fd, F_SETFL, O_NONBLOCK) == -1){
			std::cerr << "fcntl F_SETFL failed!" << std::endl;
			close(new_fd);
			return ;
		}
		Client new_client(new_fd);
		insert_client(new_fd, Fd);
		_Clients.insert(std::make_pair(new_fd, new_client));
		register_read(kq, new_fd);
	}
}

void Webserver::read_from_client(int Fd){
	std::map <int , Client>::iterator it = _Clients.find(Fd); 
	if(it != _Clients.end()){
		it->second.setLastInteraction(getTime());
		ss_request &request = it->second.get_Request();
		size_t ReadingSize = 0;
		if(request.get_header_ready() == 2)
			ReadingSize = BUFFER_SIZE;
		else
			ReadingSize = 1;
		char buff[ReadingSize + 1];
		int bytes_read = recv(Fd, &buff, ReadingSize, 0);
		if(bytes_read <= 0){
			if(bytes_read < 0)
				perror("recv");
			rmandclose_client(Fd);
			it->second.set_ReadEvent(0);
			it->second.set_WriteEvent(0);
		}else{
    		buff[bytes_read] = '\0';
			if(request.get_header_ready() == 0){
    			request.parse_request(&buff[0], bytes_read);
    			memset(buff, 0, ReadingSize);
			}
			if(request.get_header_ready() == 1){
				Config config = get_client_config(Fd, request.get_host());
				Request request1(request.get_header(), config);
				it->second.set_Response_Ptr(Response(request1,config));
				if(request1.getCL() == 0 && !request1.isChunked())
					request.set_request_ready();
				request.set_header_finished(2);
			}else if (request.get_header_ready() == 2){
				it->second.get_Response().handleResponse(buff, bytes_read);
			}
			if(request.is_request_ready() == 1 || (it->second.get_Response().stopReading() && request.get_header_ready() == 2))
			{
				if (request.is_request_ready() == 1){
					it->second.get_Response().handleResponse(NULL, 0);
				}
				request.reset();
				it->second.set_ReadEvent(0);
				it->second.set_WriteEvent(1);
				delete_read(kq, Fd);
				register_write(kq, Fd);
			}else{
				it->second.set_ReadEvent(1);
				register_read(kq, Fd);
				delete_write(kq, Fd);
			}
		}
	}
}

void Webserver::send_to_client(int Fd){
	std::map <int , Client>::iterator it = _Clients.find(Fd);
	if(it != _Clients.end()){
		it->second.setLastInteraction(getTime());
		Response &response = it->second.get_Response();
		if (!it->second.get_Response().isCgiReady())
			response.handleCgi();
		else{
			int ret = response.sendResponse(Fd);
			if(ret < 0 || (ret == 1 && response.getConection() == 0)){
				rmandclose_client(Fd);
				it->second.set_ReadEvent(0);
				it->second.set_WriteEvent(0);
			}else if(ret == 1 && response.getConection() == 1){
				response.reset();
				it->second.set_ReadEvent(1);
				it->second.set_WriteEvent(0);
				register_read(kq, Fd);
				delete_write(kq, Fd);
			}
		}
	}
}
