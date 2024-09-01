#include "../inc/Webserv.hpp"

void path_name_check(std::string path)
{
	std::string ext = ".conf";

	if(path.length() <= ext.length() || path.substr(path.length() - 5) != ext)
    {
		std::cerr << RED<< "Invalid path[" << path << "]" << RESET<< std::endl;
		std::cerr << RED << "File name and extension should be something like this 'filename.conf'" << RESET<< std::endl;
		exit(1);
	}
}

int main(int argc, char **argv){
	std::string file_path = "configs/default.conf";
	if(argc > 1)
		file_path = argv[1];
    path_name_check(file_path);
	signal(SIGPIPE, SIG_IGN);
	Server servers;
	parsing(servers, file_path);
	if(servers.get_servers().empty()){	
		std::cerr << RED<< "Empty servers pool, Get Good" <<RESET <<  std::endl;
		exit(1);
	}else{
	    Webserver webserver;
        init_sockets(webserver, servers);
        webserver.start_server();
    }
}