
#include "../../inc/Webserv.hpp"

void printRoute(const Route& route) {
    // std::cout << "    methods: " << route.get_methods() << std::endl;
    std::cout << "    root_dir: "  << route.get_roor_dir() << std::endl;
    std::cout << "    dirlst: " << (route.get_dirlst() ? "true" : "false") << std::endl;
    // std::cout << "    def_file: " << route.get_default_file() << std::endl;
	std::set<std::string> def_files = route.get_default_file();
	std::set<std::string>::iterator it = def_files.begin();
	while(it != def_files.end())
	{
		std::cout << "    def_file: " << *it << std::endl;
		++it;
	}
    // std::cout << "    cgi_extensions: " << route.get_cgi_extensions() << std::endl;
    std::cout << "    upload: " << (route.get_upload() ? "true" : "false") << std::endl;
    std::cout << "    upload_path: " << route.get_upload_path() << std::endl;
    std::cout << "    redirection: " << route.get_redirection_url() << std::endl;
}

void printConfig(const Config& config) {
    std::cout << "max_body_size: " << config.get_max_body_size() << std::endl;
    std::cout << "host: " << config.getHost() << std::endl;

    std::cout << "ports: ";
    const std::set<int>& ports = config.getPorts();
    for (std::set<int>::const_iterator it = ports.begin(); it != ports.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    std::cout << "error_pages: ";
    const std::map<int, std::string>& error_pages = config.getErrorPages();
    for (std::map<int, std::string>::const_iterator it = error_pages.begin(); it != error_pages.end(); ++it) {
        std::cout << "[" << it->first << ": " << it->second << "] ";
    }
    std::cout << std::endl;

    std::cout << "routes: " << std::endl;
    const std::map<std::string, Route>& routes = config.getRoutes();
    for (std::map<std::string, Route>::const_iterator it = routes.begin(); it != routes.end(); ++it) {
        std::cout << it->first << ":" << std::endl;
        printRoute(it->second);
    }

    std::cout << "server_names: ";
    const std::set<std::string>& server_names = config.getServerNames();
    for (std::set<std::string>::const_iterator it = server_names.begin(); it != server_names.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    std::cout << "allowed_methods: ";
    const std::set<std::string>& allowed_methods = config.getAllowedMethods();
    for (std::set<std::string>::const_iterator it = allowed_methods.begin(); it != allowed_methods.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

void init_sockets(Webserver &webserver, const Server &servers){
	struct addrinfo hints;
	struct addrinfo *servinfo;
	struct addrinfo *temp;
	int socket_fd;
	std::set<int> server_ports;
	std::vector<Config> servers_map;

	int ports_binded = 0;
	servers_map = servers.get_servers();
	std::vector<Config>::iterator it = servers_map.begin();
	while(it != servers_map.end())
	{
		std::set<std::string> server_names = it->getServerNames();
		std::set<std::string>::iterator it3 = server_names.begin();
		while(it3 != server_names.end()){
			webserver.insert_host_config(*it3, *it);
			++it3;
		}
		server_ports = it->getPorts();
		std::set<int>::iterator it1 = server_ports.begin();
		while(it1 != server_ports.end())
		{
			if(webserver.check_used_port(*it1) == 1)
			{
				memset(&hints, 0, sizeof hints);
				hints.ai_family = AF_INET;
				hints.ai_socktype = SOCK_STREAM;
				int status = getaddrinfo(it->getHost().c_str(), to_string(*it1).c_str(), &hints, &servinfo);
				if(status != 0){
					perror("getaddrinfo");
					it1++;
					continue;
				}
				status = -1;
				for(temp = servinfo; temp != NULL && status == -1; temp = temp->ai_next){
					socket_fd = socket(temp->ai_family, temp->ai_socktype, temp->ai_protocol);
					if (socket_fd == -1) {
						perror("socket");
						continue;
					}
					int opt = 1;
					setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(opt));
					if(bind(socket_fd, temp->ai_addr, temp->ai_addrlen) < 0)
					{
						perror("bind");
						close(socket_fd);
						continue;
					}
					status = 1;
					break;
				}

				if(status == 1){
					if(listen(socket_fd, 255) < 0){
						close(socket_fd);
						perror("listen");
						freeaddrinfo(servinfo);
					}else {
						ports_binded++;
						webserver.insert_port(*it1);
						webserver.inset_configandfd(socket_fd, *it);
						webserver.insert_server_sock(socket_fd);
						std::cout << BLUE <<"http://" << it->getHost() << ":"<< *it1 << " Listening for connections" << RESET << std::endl;
						freeaddrinfo(servinfo);
					}
				}else
					freeaddrinfo(servinfo);
			}
			++it1;
		}
		++it;
	}
	if(ports_binded == 0){
		webserver.close_allfds();
		log("no ports has been binded", ERROR);
		exit(1);
	}
}